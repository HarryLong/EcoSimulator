cmake_minimum_required(VERSION 2.8)
project(ECO_SIM)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/opt/Qt/5.3/gcc_64/")

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(sqlite3 REQUIRED)

set(CMAKE_AUTOMOC ON)

link_directories("${CMAKE_SOURCE_DIR}/lib/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/lib/ecodata-tracker/")
include_directories("${CMAKE_SOURCE_DIR}/include/spatial_hashmap/" "${CMAKE_SOURCE_DIR}/include/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/include/ecodata-tracker/")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCC_GL_DEBUG_ -DDEBUG_ON -Wall -g -std=c++11 -Wno-reorder -Wno-unused-variable") #DEBUG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_ON -O3 -Wall -std=c++11 -Wno-reorder -Wno-unused-variable") #DEBUG
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pg -std=c++11 -pthread -Wno-reorder") # profiling
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread -Wno-reorder") # base
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11 -pthread -Wno-reorder") #debug
#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} "-O2 -g")
#SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O2")
#SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-O0 -g")

add_executable(EcoSim utils environment_temp simulation_configuration time_manager main_window constants simulator_manager helper plant_factory plant growth_manager dice_roller plants_storage renderer overview_widget debuger render_manager environment_manager enviromnent_illumination constrainers plant_properties environment_soil_humidity start_config_dialog simulation_configuration_widgets input_widgets plant_db plant_db_editor central_widget properties_editor_widgets environment_spatial_hashmap pixel_data_translator)
target_link_libraries(EcoSim StatsAnalysisisTool EcoDataTracker Qt5::Widgets Qt5::Core Qt5::Gui sqlite3)
# Use the Widgets module from Qt 5.
