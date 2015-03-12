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
static const std::string specie_table_name = "species";
static const Column  specie_table_column_specie_name = Column(1,"specie_name");
static const std::string specie_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + specie_table_name + "( " +
                                                       column_id.name + " INTEGER PRIMARY KEY," +
                                                       specie_table_column_specie_name.name + " TEXT NOT NULL);";

/***************************
 * GROWTH PROPERTIES TABLE *
 ***************************/
static const std::string growth_properties_table_name = "growth_properties";
static const Column growth_properties_table_column_max_height = Column(1,"max_height");
static const Column growth_properties_table_column_max_canopy_width = Column(2,"max_canopy_width");
static const Column growth_properties_table_column_max_root_size = Column(3,"max_root_size");
static const std::string growth_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + growth_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       growth_properties_table_column_max_height.name + " REAL NOT NULL," +
                                                       growth_properties_table_column_max_canopy_width.name + " REAL NOT NULL," +
                                                       growth_properties_table_column_max_root_size.name + " REAL NOT NULL);";

/*********************************
 * ILLUMINATION PROPERTIES TABLE *
 *********************************/
static const std::string illumination_properties_table_name = "illumination_properties";
static const Column illumination_properties_table_column_prime_start = Column(1,"prime_start");
static const Column illumination_properties_table_column_prime_end = Column(2,"prime_end");
static const Column illumination_properties_table_column_min = Column(3,"min");
static const Column illumination_properties_table_column_max = Column(4,"max");
static const std::string illumination_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + illumination_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       illumination_properties_table_column_prime_start.name + " INT NOT NULL, " +
                                                       illumination_properties_table_column_prime_end.name + " INT NOT NULL, " +
                                                       illumination_properties_table_column_min.name + " INT NOT NULL, " +
                                                       illumination_properties_table_column_max.name + " INT NOT NULL);";

/**********************************
 * SOIL HUMIDITY PROPERTIES TABLE *
 **********************************/
static const std::string soil_humidity_properties_table_name = "soil_humidity_properties";
static const Column soil_humidity_properties_table_column_prime_start = Column(1,"prime_start");
static const Column soil_humidity_properties_table_column_prime_end = Column(2,"prime_end");
static const Column soil_humidity_properties_table_column_min = Column(3,"min");
static const Column soil_humidity_properties_table_column_max = Column(4,"max");
static const std::string soil_humidity_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + soil_humidity_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       soil_humidity_properties_table_column_prime_start.name + " INT NOT NULL," +
                                                       soil_humidity_properties_table_column_prime_end.name + " INT NOT NULL, " +
                                                       soil_humidity_properties_table_column_min.name + " INT NOT NULL, " +
                                                       soil_humidity_properties_table_column_max.name + " INT NOT NULL);";

/**********************************
 * SEEDING PROPERTIES TABLE *
 **********************************/
static const std::string seeding_properties_table_name = "seeding_properties";
static const Column seeding_properties_table_column_max_seeding_distance = Column(1,"max_seeding_distance");
static const Column seeding_properties_table_column_seeding_interval = Column(2,"seeding_interval");
static const Column seeding_properties_table_column_max_seed_count = Column(3,"max_seed_count");
static const std::string seeding_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + seeding_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       seeding_properties_table_column_max_seeding_distance.name + " INT NOT NULL," +
                                                       seeding_properties_table_column_seeding_interval.name + " INT NOT NULL, " +
                                                       seeding_properties_table_column_max_seed_count.name + " INT NOT NULL);";

/***************************
 * AGEING PROPERTIES TABLE *
 ***************************/
static const std::string ageing_properties_table_name = "ageing_properties";
static const Column ageing_properties_table_column_start_of_decline = Column(1,"start_of_decline");
static const Column ageing_properties_table_column_max_age = Column(2,"max_age");
static const std::string ageing_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + ageing_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       ageing_properties_table_column_start_of_decline.name + " INT NOT NULL," +
                                                       ageing_properties_table_column_max_age.name + " INT NOT NULL);";

/********************************
 * TEMPERATURE PROPERTIES TABLE *
 ********************************/
static const std::string temperature_properties_table_name = "temperature_properties";
static const Column temperature_properties_table_column_prime_start = Column(1,"prime_start");
static const Column temperature_properties_table_column_prime_end = Column(2,"prime_end");
static const Column temperature_properties_table_column_min = Column(3,"min");
static const Column temperature_properties_table_column_max = Column(4,"max");
static const std::string temperature_properties_table_creation_code =
                "CREATE TABLE IF NOT EXISTS " + temperature_properties_table_name + "( " +
                                                       column_id.name + " INTEGER REFERENCES " + specie_table_name + "(" + column_id.name + ") ON DELETE CASCADE," +
                                                       temperature_properties_table_column_prime_start.name + " INT NOT NULL," +
                                                       temperature_properties_table_column_prime_end.name + " INT NOT NULL," +
                                                       temperature_properties_table_column_min.name + " INT NOT NULL," +
                                                       temperature_properties_table_column_max.name + ");";

typedef std::map<int, const SpecieProperties*> SpeciePropertiesHolder;

class PlantDB {
public:
    PlantDB();

    SpeciePropertiesHolder getAllPlantData();
    std::map<int,QString> get_all_species();
    void insertNewPlantData(SpecieProperties * data);
    void updatePlantData(const SpecieProperties * data);
    void removePlant(int p_id);

private:
    void init();

    /*********************
     * SELECT STATEMENTS *
     *********************/
    std::map<int, const AgeingProperties*> get_all_ageing_properties();
    std::map<int, const GrowthProperties*> get_all_growth_properties();
    std::map<int, const IlluminationProperties*> get_all_illumination_properties();
    std::map<int, const SoilHumidityProperties*> get_all_soil_humidity_properties();
    std::map<int, const SeedingProperties*> get_all_seeding_properties();
    std::map<int, const TemperatureProperties*> get_all_temp_properties();

    /*********************
     * INSERT STATEMENTS *
     *********************/
    int insert_plant(QString name);
    void insert_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void insert_growth_properties(int id, const GrowthProperties & growth_properties);
    void insert_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void insert_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);
    void insert_seeding_properties(int id, const SeedingProperties & seeding_properties);
    void insert_temp_properties(int id, const TemperatureProperties & seeding_properties);

    /*********************
     * UPDATE STATEMENTS *
     *********************/
    void update_specie_name(int id, QString name);
    void update_ageing_properties(int id, const AgeingProperties & ageing_properties);
    void update_growth_properties(int id, const GrowthProperties & growth_properties);
    void update_illumination_properties(int id, const IlluminationProperties & illumination_properties);
    void update_soil_humidity_properties(int id, const SoilHumidityProperties & soil_humidity_properties);
    void update_seeding_properties(int id, const SeedingProperties & seeding_properties);
    void update_temp_properties(int id, const TemperatureProperties & seeding_properties);

    /*********************
     * DELETE STATEMENTS *
     *********************/
    void delete_plant(int id);

    sqlite3* open_db();
    void exit_on_error(int p_code, int p_line, char * p_error_msg = NULL);
};

#endif // PLANT_DB_H
