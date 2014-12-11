#ifndef PLANT_DB_H
#define PLANT_DB_H

#include <sqlite3.h>
#include <string>
#include "plant_properties.h"
#include <map>
#include <QString>

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
static const Column growth_properties_table_column_max_annual_vertical_growth = Column(1,"max_annual_vertical_growth");
static const Column growth_properties_table_height_to_width_multiplier = Column(2,"height_to_width_multiplier");
static const Column growth_properties_table_column_max_annual_root_growth = Column(3,"max_annual_root_growth");
static const std::string growth_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + growth_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       growth_properties_table_column_max_annual_vertical_growth.name + " REAL NOT NULL," +
                                                       growth_properties_table_height_to_width_multiplier.name + " REAL NOT NULL," +
                                                       growth_properties_table_column_max_annual_root_growth.name + " REAL NOT NULL);";

/*********************************
 * ILLUMINATION PROPERTIES TABLE *
 *********************************/
static const std::string illumination_properties_table_name = "illumination_properties";
static const Column illumination_properties_table_column_shade_tolerance = Column(1,"shade_tolerance");
static const Column illumination_properties_table_column_sensitivity = Column(2,"sensitivity");
static const std::string illumination_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + illumination_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       illumination_properties_table_column_shade_tolerance.name + " INT NOT NULL, " +
                                                       illumination_properties_table_column_sensitivity.name + " INT NOT NULL);";

/**********************************
 * SOIL HUMIDITY PROPERTIES TABLE *
 **********************************/
static const std::string soil_humidity_properties_table_name = "soil_humidity_properties";
static const Column soil_humidity_properties_table_column_prime_humidity_start = Column(1,"prime_humidity_start");
static const Column soil_humidity_properties_table_column_prime_humidity_end = Column(2,"prime_humidity_end");
static const Column soil_humidity_properties_table_column_sensitivity = Column(3,"sensitivity");
static const std::string soil_humidity_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + soil_humidity_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       soil_humidity_properties_table_column_prime_humidity_start.name + " INT NOT NULL," +
                                                       soil_humidity_properties_table_column_prime_humidity_end.name + " INT NOT NULL, " +
                                                       soil_humidity_properties_table_column_sensitivity.name + " INT NOT NULL);";


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
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       ageing_properties_table_column_prob_death_at_birth.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_prob_death_at_upper.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_start_of_prime_age.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_end_of_prime_age.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_upper_age.name + " INT NOT NULL);";

/********************************
 * TEMPERATURE PROPERTIES TABLE *
 ********************************/
static const std::string temperature_properties_table_name = "temperature_properties";
static const Column temperature_properties_table_column_range_start = Column(1,"start_range");
static const Column temperature_properties_table_column_range_end = Column(2,"end_range");
static const Column temperature_properties_table_column_sensitivity = Column(3,"sensitivity");
static const std::string temperature_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + temperature_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + plant_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       temperature_properties_table_column_range_start.name + " INT NOT NULL," +
                                                       temperature_properties_table_column_range_end.name + " INT NOT NULL," +
                                                       temperature_properties_table_column_sensitivity.name + ");";

typedef std::map<QString, const SpecieProperties*> SpeciePropertiesHolder;

class PlantDB {
public:
    PlantDB();

    SpeciePropertiesHolder getAllPlantData();
    void insertNewPlantData(SpecieProperties * data);
    void updatePlantData(const SpecieProperties * data);
    void removePlant(int p_id);

private:
    void init();

    /*********************
     * SELECT STATEMENTS *
     *********************/
    std::map<QString,int> get_all_species();
    std::map<int, const AgeingProperties*> get_all_ageing_properties();
    std::map<int, const GrowthProperties*> get_all_growth_properties();
    std::map<int, const IlluminationProperties*> get_all_illumination_properties();
    std::map<int, const SoilHumidityProperties*> get_all_soil_humidity_properties();

    /*********************
     * INSERT STATEMENTS *
     *********************/
    int insert_plant(QString name);
    void insert_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void insert_growth_properties(int id, const GrowthProperties & growth_properties);
    void insert_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void insert_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);

    /*********************
     * UPDATE STATEMENTS *
     *********************/
    int update_plant(int id, QString name);
    void update_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void update_growth_properties(int id, const GrowthProperties & growth_properties);
    void update_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void update_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);

    /*********************
     * DELETE STATEMENTS *
     *********************/
    void delete_plant(int id);

    sqlite3* open_db();
    void exit_on_error(int p_code, int p_line, char * p_error_msg = NULL);
};

#endif // PLANT_DB_H
