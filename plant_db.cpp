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

    sqlite3_close(db);

    std::cout << "All database tables created successfully!" << std::endl;
}

/*********************
 * SELECT STATEMENTS *
 *********************/
PlantDataHolder PlantDB::getAllPlantData()
{
    PlantDataHolder ret;

    std::map<std::string,int> specie_name_to_id_pairs (get_all_species());

    for(auto it (specie_name_to_id_pairs.begin()); it != specie_name_to_id_pairs.end(); it++)
    {
        PlantData pd(it->first, it->second);;
        ret.insert(std::pair<std::string,PlantData>(it->first,pd));
    }

    return ret;
}

std::map<std::string,int> PlantDB::get_all_species()
{
    sqlite3 * db (open_db());
    char * error_msg = 0;
    sqlite3_stmt * statement;

    static const std::string sql = "SELECT * FROM " + plant_table_name + ";";

    std::map<std::string,int> specie_name_to_id_pairs;

    // Prepare the statement
    int rc (sqlite3_prepare_v2(db, sql.c_str(),-1/*null-terminated*/,&statement,NULL));
    exit_on_error(rc, __LINE__, error_msg);

    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        int id;
        const char * plant_name;

        for(int c (0); c < sqlite3_column_count(statement); c++)
        {
            std::cout << "Column name: " << sqlite3_column_name(statement, c)  << std::endl;

            if(c == column_id.index)
                id = sqlite3_column_int(statement,c);
            else if(c == plant_table_column_specie_name.index)
                plant_name = reinterpret_cast<const char*>(sqlite3_column_text(statement,c));
        }
        std::cout << "Queried id: " << id << std::endl;
        std::cout << "Queried plant name: " << plant_name << std::endl;
        specie_name_to_id_pairs.insert(std::pair<std::string,int>(std::string(plant_name), id));
    }

    return specie_name_to_id_pairs;
}

/*********************
 * INSERT STATEMENTS *
 *********************/
int PlantDB::insert_plant(std::string name)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    static const std::string sql = "INSERT INTO " + plant_table_name + " (" +
            plant_table_column_specie_name.name + "," + ")" +
            "VALUES (" +
            name + ");";


    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);

    return sqlite3_last_insert_rowid(db);
}

void PlantDB::insert_ageing_properties(int id, const AgeingProperties & ageing_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    const static std::string static_sql = "INSERT INTO " + ageing_properties_table_name + " (" +
            column_id.name + "," +
            ageing_properties_table_column_prob_death_at_birth.name + "," +
            ageing_properties_table_column_prob_death_at_upper.name + "," +
            ageing_properties_table_column_start_of_prime_age.name + "," +
            ageing_properties_table_column_end_of_prime_age.name + "," +
            ageing_properties_table_column_upper_age.name + ")" +
            "VALUES (";

    std::string sql (static_sql);
    sql += std::to_string(id) + "," +
          std::to_string(ageing_properties.probability_of_death_at_birth) + "," +
          std::to_string(ageing_properties.probability_of_death_at_upper) + "," +
          std::to_string(ageing_properties.prime_age_start) + "," +
          std::to_string(ageing_properties.prime_age_end) + "," +
          std::to_string(ageing_properties.upper_age) + ");";


    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::insert_growth_properties(int id, const GrowthProperties & growth_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    static const std::string static_sql = "INSERT INTO " + growth_properties_table_name + " (" +
            column_id.name + "," +
            growth_properties_table_column_max_monthly_vertical_growth.name + "," +
            growth_properties_table_column_max_monthly_canopy_growth.name + "," +
            growth_properties_table_column_max_monthly_root_growth.name +  ")" +
            "VALUES (";

    std::string sql (static_sql);
    sql += std::to_string(id) + "," +
           std::to_string(growth_properties.max_monthly_vertical_growth) + "," +
           std::to_string(growth_properties.max_monthly_canopy_growth) + "," +
           std::to_string(growth_properties.max_monthly_root_growth) + ");";


    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::insert_illumination_properties(int id, const IlluminationProperties & illumination_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    static const std::string static_sql = "INSERT INTO " + illumination_properties_table_name + " (" +
            column_id.name + "," +
            illumination_properties_table_column_start_of_negative_impact.name + "," +
            illumination_properties_table_column_prob_death_in_shade.name +  ")" +
            "VALUES (";

    std::string sql (static_sql);
    sql += std::to_string(id) + "," +
           std::to_string(illumination_properties.shadowed_percentage_start_of_negative_impact) + "," +
           std::to_string(illumination_properties.probability_of_death_in_complete_shade) + ");";

    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::insert_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    static const std::string static_sql = "INSERT INTO " + soil_humidity_properties_table_name + " (" +
            column_id.name + "," +
            soil_humidity_properties_table_column_prime_humidity_start.name + "," +
            soil_humidity_properties_table_column_prime_humidity_end.name+  ")" +
            "VALUES (";

    std::string sql (static_sql);
    sql += std::to_string(id) + "," +
           std::to_string(soil_humidity_properties.soil_humidity_percentage_prime_start) + "," +
           std::to_string(soil_humidity_properties.soil_humidity_percentage_prime_end) + ");";


    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

/*********************
 * UPDATE STATEMENTS *
 *********************/
int PlantDB::update_plant(int id, std::string name)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    std::string sql = "UPDATE " + plant_table_name + " SET " +
                            plant_table_column_specie_name.name + " = " + name +
                      " WHERE " + column_id.name + " = " + std::to_string(id) + ";";


    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);

    return sqlite3_last_insert_rowid(db);
}

void PlantDB::update_ageing_properties(int id, const AgeingProperties & ageing_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    const std::string sql = "UPDATE " + ageing_properties_table_name + " SET " +
                ageing_properties_table_column_prob_death_at_birth.name + " = " + std::to_string(ageing_properties.probability_of_death_at_birth) + "," +
                ageing_properties_table_column_prob_death_at_upper.name + " = " + std::to_string(ageing_properties.probability_of_death_at_upper) + "," +
                ageing_properties_table_column_start_of_prime_age.name + " = " + std::to_string(ageing_properties.prime_age_start) + "," +
                ageing_properties_table_column_end_of_prime_age.name + " = " + std::to_string(ageing_properties.prime_age_end) + "," +
                ageing_properties_table_column_upper_age.name + " = " + std::to_string(ageing_properties.upper_age) +
            " WHERE " + column_id.name + " = " + std::to_string(id) + ";";

    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::update_growth_properties(int id, const GrowthProperties & growth_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    const std::string sql = "UPDATE " + growth_properties_table_name + " SET " +
                growth_properties_table_column_max_monthly_vertical_growth.name + " = " + std::to_string(growth_properties.max_monthly_vertical_growth) + "," +
                growth_properties_table_column_max_monthly_canopy_growth.name + " = " + std::to_string(growth_properties.max_monthly_canopy_growth) + "," +
                growth_properties_table_column_max_monthly_root_growth.name +  " = " + std::to_string(growth_properties.max_monthly_root_growth) +
            " WHERE " + column_id.name + " = " + std::to_string(id) + ";";

    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::update_illumination_properties(int id, const IlluminationProperties & illumination_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    const std::string sql = "UPDATE " + illumination_properties_table_name + " SET " +
            illumination_properties_table_column_start_of_negative_impact.name + " = " + std::to_string(illumination_properties.shadowed_percentage_start_of_negative_impact) + "," +
            illumination_properties_table_column_prob_death_in_shade.name +  " = " + std::to_string(illumination_properties.probability_of_death_in_complete_shade) +
            " WHERE " + column_id.name + " = " + std::to_string(id) + ";";

    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

void PlantDB::update_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties)
{
    sqlite3 * db (open_db());
    char *error_msg = 0;

    static const std::string sql = "UPDATE " + soil_humidity_properties_table_name + " SET " +
            soil_humidity_properties_table_column_prime_humidity_start.name + " = " + std::to_string(soil_humidity_properties.soil_humidity_percentage_prime_start) + "," +
            soil_humidity_properties_table_column_prime_humidity_end.name +  " = " + std::to_string(soil_humidity_properties.soil_humidity_percentage_prime_end) +
            " WHERE " + column_id.name + " = " + std::to_string(id) + ";";

    int rc (sqlite3_exec(db, sql.c_str(), NULL, 0, &error_msg));

    exit_on_error(rc, __LINE__, error_msg);
}

/******************
 * HELPER METHODS *
 ******************/
void PlantDB::exit_on_error(int p_code, int p_line,  char * p_error_msg)
{
    if(p_code != SQLITE_OK)
    {
        std::cerr << "Database failure!" << std::endl;
        std::cerr << "File: " << __FILE__ << std::endl;
        std::cerr << "Line: " << __LINE__ << std::endl;
        std::cerr << "Error message:" << p_error_msg << std::endl;
        sqlite3_free(p_error_msg);
        exit(1);
    }
}
