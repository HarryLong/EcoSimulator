cmake_minimum_required(VERSION 2.8)
project(ECO_SIM)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(sqlite3 REQUIRED)

set(CMAKE_AUTOMOC ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCC_GL_DEBUG_ -DDEBUG_ON -Wall -g -std=c++11") #DEBUG
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11 -pthread -Wno-reorder")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-O0 -g")

add_executable(EcoSim time_manager main_window constants simulator_manager helper plant_factory plant growth_manager dice_roller plants_storage renderer overview_widget debuger render_manager environment_manager enviromnent_illumination constrainers plant_properties environment_soil_humidity start_config_dialog plant_configuration_widget input_widgets plant_db plant_db_editor central_widget spatial_hashmap)
target_link_libraries(EcoSim Qt5::Widgets Qt5::Core Qt5::Gui sqlite3) 
# Use the Widgets module from Qt 5.
