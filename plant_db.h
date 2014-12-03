#ifndef PLANT_DB_H
#define PLANT_DB_H

#include <sqlite3.h>
#include <string>
#include "plant_properties.h"
#include <map>

struct Column{
    int index;
    std::string name;

    Column(int index, std::string name) : index(index), name(name) {}
};

/*********************
 * GENERIC CONSTANTS *
 *********************/
static const std::string db_name = "ECOSIMULATOR.db";
static const Column column_id = Column(0,"_id");

/*****************
 * SPECIES TABLE *
 *****************/
static const std::string plant_table_name = "species";
static const Column  plant_table_column_specie_name = Column(1,"specie_name");
static const std::string plant_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + plant_table_name + "( " +
                                                       column_id.name + " INTEGER PRIMARY KEY," +
                                                       plant_table_column_specie_name.name + " TEXT NOT NULL);";

/***************************
 * GROWTH PROPERTIES TABLE *
 ***************************/
static const std::string growth_properties_table_name = "growth_properties";
static const Column growth_properties_table_column_max_monthly_vertical_growth = Column(1,"max_monthly_vertical_growth");
static const Column growth_properties_table_column_max_monthly_canopy_growth = Column(2,"max_monthly_canopy_growth");
static const Column growth_properties_table_column_max_monthly_root_growth = Column(3,"max_monthly_root_growth");
static const std::string growth_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + growth_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ")," +
                                                       growth_properties_table_column_max_monthly_vertical_growth.name + " REAL NOT NULL," +
                                                       growth_properties_table_column_max_monthly_canopy_growth.name + " REAL NOT NULL," +
                                                       growth_properties_table_column_max_monthly_root_growth.name + " REAL NOT NULL);";

/*********************************
 * ILLUMINATION PROPERTIES TABLE *
 *********************************/
static const std::string illumination_properties_table_name = "illumination_properties";
static const Column illumination_properties_table_column_start_of_negative_impact = Column(1,"start_of_negative_impact");
static const Column illumination_properties_table_column_prob_death_in_shade = Column(2,"prob_death_in_shade");
static const std::string illumination_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + illumination_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ")," +
                                                       illumination_properties_table_column_start_of_negative_impact.name + " INT NOT NULL," +
                                                       illumination_properties_table_column_prob_death_in_shade.name + " INT NOT NULL);";

/**********************************
 * SOIL HUMIDITY PROPERTIES TABLE *
 **********************************/
static const std::string soil_humidity_properties_table_name = "soil_humidity_properties";
static const Column soil_humidity_properties_table_column_prime_humidity_start = Column(1,"prime_humidity_start");
static const Column soil_humidity_properties_table_column_prime_humidity_end = Column(2,"prime_humidity_end");
static const std::string soil_humidity_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + soil_humidity_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ")," +
                                                       soil_humidity_properties_table_column_prime_humidity_start.name + " INT NOT NULL," +
                                                       soil_humidity_properties_table_column_prime_humidity_end.name + " INT NOT NULL);";


/***************************
 * AGEING PROPERTIES TABLE *
 ***************************/
static const std::string ageing_properties_table_name = "ageing_properties";
static const Column ageing_properties_table_column_prob_death_at_birth = Column(1,"prob_death_at_birth");
static const Column ageing_properties_table_column_prob_death_at_upper = Column(2,"prob_death_at_upper");
static const Column ageing_properties_table_column_start_of_prime_age = Column(3,"prime_age_start");
static const Column ageing_properties_table_column_end_of_prime_age = Column(4,"prime_age_end");
static const Column ageing_properties_table_column_upper_age = Column(5,"upper_end_age");
static const std::string ageing_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + ageing_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ")," +
                                                       ageing_properties_table_column_prob_death_at_birth.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_prob_death_at_upper.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_start_of_prime_age.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_end_of_prime_age.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_upper_age.name + " INT NOT NULL);";

struct PlantData{
    int id;
    std::string name;
//    AgeingProperties ageing_properties;
//    GrowthProperties growth_properties;
//    IlluminationProperties illumination_properties;
//    SoilHumidityProperties soil_humidiry_properties;

    PlantData(std::string name, int id) : id(id), name(name) {}
};
typedef std::map<std::string, PlantData> PlantDataHolder;

class PlantDB {
public:
    PlantDB();
    PlantDataHolder getAllPlantData();
    void insertNewPlantData(const PlantData & data);

private:
    void init();

    /*********************
     * SELECT STATEMENTS *
     *********************/
    std::map<std::string,int> get_all_species();

    /*********************
     * INSERT STATEMENTS *
     *********************/
    int insert_plant(std::string name);
    void insert_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void insert_growth_properties(int id, const GrowthProperties & growth_properties);
    void insert_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void insert_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);

    /*********************
     * UPDATE STATEMENTS *
     *********************/
    int update_plant(int id, std::string name);
    void update_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void update_growth_properties(int id, const GrowthProperties & growth_properties);
    void update_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void update_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);

    sqlite3* open_db();


    void exit_on_error(int p_code, int p_line, char * p_error_msg);
};

#endif // PLANT_DB_H
