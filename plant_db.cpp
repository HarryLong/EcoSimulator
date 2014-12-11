#include "plant_db.h"
#include <iostream>
#include <QString>

PlantDB::PlantDB()
{
    init();
}

/****************************
 * OPEN DATABASE CONNECTION *
 ****************************/
sqlite3 * PlantDB::open_db()
{
    sqlite3 * db;
    exit_on_error ( sqlite3_open(db_name.c_str(), &db), __LINE__, "" );
    exit_on_error( sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0), __LINE__);
    return db;
}

void PlantDB::init()
{
    char *error_msg = 0;
    sqlite3 * db (open_db());

    // Create tables if they don't exists
    exit_on_error ( sqlite3_open(db_name.c_str(), &db), __LINE__, "" );
    std::cout << "Database created succesfully" << std::endl;

    // Specie Table
    int rc (sqlite3_exec(db, plant_table_creation_code.c_str(), NULL, 0, &error_msg));
    exit_on_error ( rc, __LINE__, error_msg );

    // growth properties table
    rc = sqlite3_exec(db, growth_properties_table_creation_code.c_str(), NULL, 0, &error_msg);
    exit_on_error ( rc, __LINE__, error_msg );

    // illumination properties table
    rc = sqlite3_exec(db, illumination_properties_table_creation_code.c_str(), NULL, 0, &error_msg);
    exit_on_error ( rc, __LINE__, error_msg );

    // Ageing properties table
    rc = sqlite3_exec(db, ageing_properties_table_creation_code.c_str(), NULL, 0, &error_msg);
    exit_on_error ( rc, __LINE__, error_msg );

    // Soil Humidity properties table
    rc = sqlite3_exec(db, soil_humidity_properties_table_creation_code.c_str(), NULL, 0, &error_msg);
    exit_on_error ( rc, __LINE__, error_msg );

    sqlite3_close(db);

    std::cout << "All database tables created successfully!" << std::endl;
}

/****************************
 * INTERFACE WITH THE WORLD *
 ****************************/
SpeciePropertiesHolder PlantDB::getAllPlantData()
{
    SpeciePropertiesHolder ret;

    std::map<QString,int> specie_names(get_all_species());
    std::map<int, const AgeingProperties*> ageing_properties(get_all_ageing_properties());
    std::map<int, const GrowthProperties*> growth_properties(get_all_growth_properties());
    std::map<int, const IlluminationProperties*> illumination_properties(get_all_illumination_properties());
    std::map<int, const SoilHumidityProperties*> soil_humidity_properties(get_all_soil_humidity_properties());

    for(auto it (specie_names.begin()); it != specie_names.end(); it++)
    {
        int id(it->second);
        QString name(it->first);
        const AgeingProperties* ageing_property(ageing_properties.find(id)->second);
        const GrowthProperties* growth_property(growth_properties.find(id)->second);
        const IlluminationProperties* illumination_property(illumination_properties.find(id)->second);
        const SoilHumidityProperties* soil_humidity_property(soil_humidity_properties.find(id)->second);

        ret.insert(std::pair<QString, SpecieProperties*>(name,
                                                      new SpecieProperties(name,
                                                                      id,
                                                                      ageing_property,
                                                                      growth_property,
                                                                      illumination_property,
                                                                      soil_humidity_property)));
    }

    return ret;
}

void PlantDB::insertNewPlantData(SpecieProperties * data)
{
    data->specie_id = insert_plant(data->specie_name);
    insert_ageing_properties(data->specie_id, *(data->ageing_properties));
    insert_growth_properties(data->specie_id, *(data->growth_properties));
    insert_illumination_properties(data->specie_id, *(data->illumination_properties));
    insert_soil_humidity_properties(data->specie_id, *(data->soil_humidiry_properties));
}

void PlantDB::updatePlantData(const SpecieProperties * data)
{
    update_plant(data->specie_id, data->specie_name);
    update_ageing_properties(data->specie_id, *(data->ageing_properties));
    update_growth_properties(data->specie_id, *(data->growth_properties));
    update_illumination_properties(data->specie_id, *(data->illumination_properties));
    update_soil_humidity_properties(data->specie_id, *(data->soil_humidiry_properties));
}

void PlantDB::removePlant(int p_id)
{
    delete_plant(p_id);
}

/*********************
 * SELECT STATEMENTS *
 *********************/
std::map<QString,int> PlantDB::get_all_species()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + plant_table_name + ";";

    std::map<QString,int> specie_name_to_id_pairs;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        const char * plant_name;

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == plant_table_column_specie_name.index)
                plant_name = reinterpret_cast<const char*>(sqlite3_column_text(statement,c));
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        specie_name_to_id_pairs.insert(std::pair<QString,int>(QString(plant_name), id));
    }

    return specie_name_to_id_pairs;
}

std::map<int, const AgeingProperties*> PlantDB::get_all_ageing_properties()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + ageing_properties_table_name + ";";

    std::map<int,const AgeingProperties*> ret;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        int probability_of_death_at_birth;
        int probability_of_death_at_upper;
        int prime_age_start;
        int prime_age_end;
        int upper_age;

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == ageing_properties_table_column_prob_death_at_birth.index)
                probability_of_death_at_birth = sqlite3_column_int(statement,c);
            else if(c == ageing_properties_table_column_prob_death_at_upper.index)
                probability_of_death_at_upper = sqlite3_column_int(statement,c);
            else if(c == ageing_properties_table_column_start_of_prime_age.index)
                prime_age_start = sqlite3_column_int(statement,c);
            else if(c == ageing_properties_table_column_end_of_prime_age.index)
                prime_age_end = sqlite3_column_int(statement,c);
            else if(c == ageing_properties_table_column_upper_age.index)
                upper_age = sqlite3_column_int(statement,c);
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        ret.insert(std::pair<int,const AgeingProperties*>(id, new AgeingProperties(probability_of_death_at_birth,
                                                                        probability_of_death_at_upper,
                                                                        prime_age_start,
                                                                        prime_age_end,
                                                                        upper_age)));
    }

    return ret;
}

std::map<int, const GrowthProperties*> PlantDB::get_all_growth_properties()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + growth_properties_table_name + ";";

    std::map<int,const GrowthProperties*> ret;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        float max_monthly_vertical_growth; // cm per month
        float max_monthly_root_growth; // cm per month
        float height_to_width_multiplier; // cm per month

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == growth_properties_table_column_max_annual_vertical_growth.index)
                max_monthly_vertical_growth = sqlite3_column_double(statement,c);
            else if(c == growth_properties_table_height_to_width_multiplier.index)
                height_to_width_multiplier = sqlite3_column_double(statement,c);
            else if(c == growth_properties_table_column_max_annual_root_growth.index)
                max_monthly_root_growth = sqlite3_column_double(statement,c);
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        ret.insert(std::pair<int,const GrowthProperties*>(id, new GrowthProperties(max_monthly_vertical_growth,
                                                                        max_monthly_root_growth,
                                                                        height_to_width_multiplier)));
    }

    return ret;
}

std::map<int, const IlluminationProperties*> PlantDB::get_all_illumination_properties()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + illumination_properties_table_name + ";";

    std::map<int,const IlluminationProperties*> ret;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        int shade_tolerance;
        int sensitivity;

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == illumination_properties_table_column_shade_tolerance.index)
                shade_tolerance = sqlite3_column_int(statement,c);
            else if(c == illumination_properties_table_column_sensitivity.index)
                sensitivity = sqlite3_column_int(statement,c);
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        ret.insert(std::pair<int,const IlluminationProperties*>(id, new IlluminationProperties(shade_tolerance,sensitivity)));
    }

    return ret;
}

std::map<int, const SoilHumidityProperties*> PlantDB::get_all_soil_humidity_properties()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + soil_humidity_properties_table_name + ";";

    std::map<int,const SoilHumidityProperties*> ret;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        int soil_humidity_percentage_prime_start;
        int soil_humidity_percentage_prime_end;
        int sensitivity;

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == soil_humidity_properties_table_column_prime_humidity_start.index)
                soil_humidity_percentage_prime_start = sqlite3_column_int(statement,c);
            else if(c == soil_humidity_properties_table_column_prime_humidity_end.index)
                soil_humidity_percentage_prime_end = sqlite3_column_int(statement,c);
            else if(c == soil_humidity_properties_table_column_sensitivity.index)
                sensitivity = sqlite3_column_int(statement,c);
            else
            {
                std::cerr << "Unknown column: " << sqlite3_column_name(statement,c) <<
                             " in file " << __FILE__ << " and line " << __LINE__ << std::endl;
                exit(1);
            }
        }
        ret.insert(std::pair<int,const SoilHumidityProperties*>(id, new SoilHumidityProperties(soil_humidity_percentage_prime_start,
                                                                                    soil_humidity_percentage_prime_end,
                                                                                    sensitivity)));
    }

    // finalise the statement
    sqlite3_finalize(statement);

    return ret;
}

/*********************
 * INSERT STATEMENTS *
 *********************/
int PlantDB::insert_plant(QString name)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "INSERT INTO " + plant_table_name + " ( " +
            plant_table_column_specie_name.name + " )" +
            " VALUES ( ? );";


    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    QByteArray name_byte_array ( name.toUtf8());
    const char* name_c_string ( name_byte_array.constData());
    exit_on_error(sqlite3_bind_text(statement, plant_table_column_specie_name.index, name_c_string, -1/*null-terminated*/,NULL), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    int inserted_row_id(sqlite3_last_insert_rowid(db));

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__, error_msg);

    return inserted_row_id;
}

void PlantDB::insert_ageing_properties(int id, const AgeingProperties & ageing_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const static std::string sql = "INSERT INTO " + ageing_properties_table_name + " (" +
            column_id.name + "," +
            ageing_properties_table_column_prob_death_at_birth.name + "," +
            ageing_properties_table_column_prob_death_at_upper.name + "," +
            ageing_properties_table_column_start_of_prime_age.name + "," +
            ageing_properties_table_column_end_of_prime_age.name + "," +
            ageing_properties_table_column_upper_age.name + ")" +
            " VALUES ( ?, ?, ?, ?, ?, ?);";


    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    exit_on_error(sqlite3_bind_int(statement, column_id.index+1, id), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ageing_properties_table_column_prob_death_at_birth.index+1, ageing_properties.probability_of_death_at_birth), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ageing_properties_table_column_prob_death_at_upper.index+1, ageing_properties.probability_of_death_at_upper), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ageing_properties_table_column_start_of_prime_age.index+1, ageing_properties.prime_age_start), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ageing_properties_table_column_end_of_prime_age.index+1, ageing_properties.prime_age_end), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, ageing_properties_table_column_upper_age.index+1, ageing_properties.upper_age), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::insert_growth_properties(int id, const GrowthProperties & growth_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "INSERT INTO " + growth_properties_table_name + " (" +
            column_id.name + "," +
            growth_properties_table_column_max_annual_vertical_growth.name + "," +
            growth_properties_table_height_to_width_multiplier.name + "," +
            growth_properties_table_column_max_annual_root_growth.name +  ")" +
            " VALUES ( ?, ?, ?, ?);";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    exit_on_error(sqlite3_bind_int(statement, column_id.index+1, id), __LINE__);
    exit_on_error(sqlite3_bind_double(statement, growth_properties_table_column_max_annual_vertical_growth.index+1, growth_properties.max_annual_vertical_growth), __LINE__);
    exit_on_error(sqlite3_bind_double(statement, growth_properties_table_height_to_width_multiplier.index+1, growth_properties.height_to_width_multiplier), __LINE__);
    exit_on_error(sqlite3_bind_double(statement, growth_properties_table_column_max_annual_root_growth.index+1, growth_properties.max_annual_root_growth), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::insert_illumination_properties(int id, const IlluminationProperties & illumination_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "INSERT INTO " + illumination_properties_table_name + " (" +
            column_id.name + "," +
            illumination_properties_table_column_shade_tolerance.name + "," +
            illumination_properties_table_column_sensitivity.name + ")" +
            " VALUES ( ?, ?, ?);";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    exit_on_error(sqlite3_bind_int(statement, column_id.index+1, id), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, illumination_properties_table_column_shade_tolerance.index+1,
                                   illumination_properties.shade_tolerance), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, illumination_properties_table_column_sensitivity.index+1,
                                   illumination_properties.sensitivity), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::insert_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "INSERT INTO " + soil_humidity_properties_table_name + " (" +
            column_id.name + "," +
            soil_humidity_properties_table_column_prime_humidity_start.name + "," +
            soil_humidity_properties_table_column_prime_humidity_end.name+  "," +
            soil_humidity_properties_table_column_sensitivity.name+  ")" +
            " VALUES ( ?, ?, ?, ?);";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    exit_on_error(sqlite3_bind_int(statement, column_id.index+1, id), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, soil_humidity_properties_table_column_prime_humidity_start.index+1, soil_humidity_properties.soil_humidity_percentage_prime_start), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, soil_humidity_properties_table_column_prime_humidity_end.index+1, soil_humidity_properties.soil_humidity_percentage_prime_end), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, soil_humidity_properties_table_column_sensitivity.index+1, soil_humidity_properties.sensitivity), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

/*********************
 * UPDATE STATEMENTS *
 *********************/
int PlantDB::update_plant(int id, QString name)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const static  std::string sql = "UPDATE " + plant_table_name + " SET " +
                            plant_table_column_specie_name.name + " = ? " +
                      " WHERE " + column_id.name + " = ? ;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    QByteArray name_byte_array ( name.toUtf8());
    const char* name_c_string ( name_byte_array.constData());

    int bind_index (plant_table_column_specie_name.index);
    exit_on_error(sqlite3_bind_text(statement, bind_index++, name_c_string,-1/*null-terminated*/, NULL), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__, error_msg);
}

void PlantDB::update_ageing_properties(int id, const AgeingProperties & ageing_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const static std::string sql = "UPDATE " + ageing_properties_table_name + " SET " +
                ageing_properties_table_column_prob_death_at_birth.name + " = ?, " +
                ageing_properties_table_column_prob_death_at_upper.name + " = ?," +
                ageing_properties_table_column_start_of_prime_age.name + " = ?," +
                ageing_properties_table_column_end_of_prime_age.name + " = ?," +
                ageing_properties_table_column_upper_age.name + " = ?" +
            " WHERE " + column_id.name + " = ? ;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    int bind_index (ageing_properties_table_column_prob_death_at_birth.index);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, ageing_properties.probability_of_death_at_birth), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, ageing_properties.probability_of_death_at_upper), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, ageing_properties.prime_age_start), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, ageing_properties.prime_age_end), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, ageing_properties.upper_age), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::update_growth_properties(int id, const GrowthProperties & growth_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const std::string sql = "UPDATE " + growth_properties_table_name + " SET " +
                growth_properties_table_column_max_annual_vertical_growth.name + " = ?," +
                growth_properties_table_height_to_width_multiplier.name + " = ?," +
                growth_properties_table_column_max_annual_root_growth.name +  " = ?" +
            " WHERE " + column_id.name + " = ?;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    int bind_index (growth_properties_table_column_max_annual_vertical_growth.index);
    exit_on_error(sqlite3_bind_double(statement, bind_index++, growth_properties.max_annual_vertical_growth), __LINE__);
    exit_on_error(sqlite3_bind_double(statement, bind_index++, growth_properties.height_to_width_multiplier), __LINE__);
    exit_on_error(sqlite3_bind_double(statement, bind_index++, growth_properties.max_annual_root_growth), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::update_illumination_properties(int id, const IlluminationProperties & illumination_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    const std::string sql = "UPDATE " + illumination_properties_table_name + " SET " +
            illumination_properties_table_column_shade_tolerance.name + " = ?,"+
            illumination_properties_table_column_sensitivity.name + " = ?"+
            " WHERE " + column_id.name + " = ?;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    int bind_index (illumination_properties_table_column_shade_tolerance.index);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, illumination_properties.shade_tolerance), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, illumination_properties.sensitivity), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

void PlantDB::update_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "UPDATE " + soil_humidity_properties_table_name + " SET " +
            soil_humidity_properties_table_column_prime_humidity_start.name + " = ?," +
            soil_humidity_properties_table_column_prime_humidity_end.name +  " = ?," +
            soil_humidity_properties_table_column_sensitivity.name +  " = ?" +
            " WHERE " + column_id.name + " = ?;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    int bind_index (soil_humidity_properties_table_column_prime_humidity_start.index);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, soil_humidity_properties.soil_humidity_percentage_prime_start), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, soil_humidity_properties.soil_humidity_percentage_prime_end), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, soil_humidity_properties.sensitivity), __LINE__);
    exit_on_error(sqlite3_bind_int(statement, bind_index++, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

/*********************
 * DELETE STATEMENTS *
 *********************/
void PlantDB::delete_plant(int id)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "DELETE FROM " + plant_table_name + " WHERE " +
            column_id.name + " = ?;";

    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // Perform binding
    exit_on_error(sqlite3_bind_int(statement, 1, id), __LINE__);

    // Commit
    exit_on_error(sqlite3_step(statement), __LINE__);

    // finalise the statement
    exit_on_error(sqlite3_finalize(statement), __LINE__);
}

/******************
 * HELPER METHODS *
 ******************/
void PlantDB::exit_on_error(int p_code, int p_line,  char * p_error_msg)
{
    if(p_code != SQLITE_OK && p_code != SQLITE_DONE)
    {        
        std::cerr << "Database failure!" << std::endl;
        std::cerr << "Error code: " << p_code << std::endl;
        std::cerr << "File: " << __FILE__ << std::endl;
        std::cerr << "Line: " << p_line << std::endl;
        std::cerr << "Error message:" << p_error_msg << std::endl;
        sqlite3_free(p_error_msg);
        exit(1);
    }
}
