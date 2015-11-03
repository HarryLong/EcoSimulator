cmake_minimum_required(VERSION 2.8)
project(ECO_SIM_LIB)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/opt/Qt/5.3/gcc_64/")

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(OpenMP REQUIRED)
##find_package(PlantDB REQUIRED)

set(LIBS ${LIBS} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} PlantDB EcoDataTracker RadialDistributionAnalyser)
set(INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/spatial_hashmap/" ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

#"${CMAKE_SOURCE_DIR}/include/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/include/ecodata-tracker/"
#StatsAnalysisisTool EcoDataTracker

set(CMAKE_AUTOMOC ON)

#add_subdirectory(shared_lib)

#SET(DB_SRC_FILES db/plant_db db/plant_db_editor db/plant_db_editor_widgets db/plant_properties)
SET(RESOURCES_SRC_FILES ../resources/environment_manager ../resources/environment_illumination ../resources/environment_soil_humidity ../resources/environment_temp)
SET(DATA_HOLDERS_SRC_FILES ../data_holders/environment_spatial_hashmap ../data_holders/plant_rendering_data ../data_holders/plant_rendering_data_container)
set(SIMULATOR_CORE_SRC_FILES ../simulator/core/simulation_configuration ../simulator/core/simulator_manager)
set(SIMULATOR_PLANTS_SRC_FILES ../simulator/plants/plant_factory ../simulator/plants/plants_storage ../simulator/plants/plant ../simulator/plants/growth_manager ../simulator/plants/constrainers)
SET(MATH_SRC_FILES ../math/linear_equation ../math/dice_roller)
SET(UTILS_SRC_FILES ../utils/utils ../utils/time_manager ../utils/debuger ../utils/callback_listener)

SET(CORE_HEADER_FILES ../simulator/core/simulator_manager.h ../simulator/core/simulation_configuration.h)
SET(UTILS_HEADER_FILES ../utils/time_manager.h)
SET(PLANTS_HEADER_FILES ../simulator/plants/plant_factory.h ../simulator/plants/plants_storage.h ../simulator/plants/plant.h ../simulator/plants/growth_manager.h
../simulator/plants/constrainers.h)
SET(RESOURCES_HEADER_FILES ../resources/environment_manager.h ../resources/environment_illumination.h ../resources/environment_soil_humidity.h
../resources/environment_temp.h)
SET(MATH_HEADER_FILES ../math/dice_roller.h ../math/linear_equation.h)
SET(DATA_HOLDERS_HEADER_FILES ../data_holders/environment_spatial_hashmap.h)

set(LIB_SRC_FILES
${RESOURCES_SRC_FILES}
${DATA_HOLDERS_SRC_FILES}
${SIMULATOR_CORE_SRC_FILES}
${SIMULATOR_PLANTS_SRC_FILES}
${MATH_SRC_FILES}
${UTILS_SRC_FILES})

link_directories("${CMAKE_SOURCE_DIR}/lib/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/lib/ecodata-tracker/")
include_directories(${INCLUDE_DIRECTORIES})

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -std=c++11 -DLIB_MODE -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11 -DLIB_MODE -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 --std=c++11 -DLIB_MODE -Wno-write-strings")

#SET(CMAKE_BUILD_TYPE DEBUG)
#SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)

add_library(EcoSimulator SHARED ${LIB_SRC_FILES} )
target_link_libraries(EcoSimulator ${LIBS})

# INSTALL LIB
install(TARGETS EcoSimulator
        LIBRARY DESTINATION lib
        CONFIGURATIONS RELWITHDEBINFO)

# INSTAL CORE HEADER FILES
install(FILES ${CORE_HEADER_FILES}
        DESTINATION include/ecosimulator/simulator/core)
# INSTAL UTILS HEADER FILES
install(FILES ${UTILS_HEADER_FILES}
        DESTINATION include/ecosimulator/utils)
# INSTAL PLANTS HEADER FILES
install(FILES ${PLANTS_HEADER_FILES}
        DESTINATION include/ecosimulator/simulator/plants)
# INSTALL RESOURCE HEADER FILES
install(FILES ${RESOURCES_HEADER_FILES}
        DESTINATION include/ecosimulator/resources)
# INSTALL MATH HEADER FILES
install(FILES ${MATH_HEADER_FILES}
        DESTINATION include/ecosimulator/math)
# INSTALL DATA HOLDERS HEADER FILES
install(FILES ${DATA_HOLDERS_HEADER_FILES}
        DESTINATION include/ecosimulator/data_holders)
