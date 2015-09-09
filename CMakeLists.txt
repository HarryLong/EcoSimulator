cmake_minimum_required(VERSION 2.8)
project(ECO_SIM)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/opt/Qt/5.3/gcc_64/")

# QT Imports
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(OpenMP REQUIRED)
#find_package(PlantDB REQUIRED)

set(LIBS ${LIBS} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} PlantDB RadialDistributionAnalyser EcoDataTracker)
set(INCLUDE_DIRECTORIES ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

#"${CMAKE_SOURCE_DIR}/include/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/include/ecodata-tracker/"
#StatsAnalysisisTool EcoDataTracker

set(CMAKE_AUTOMOC ON)

add_subdirectory(shared_lib)

#SET(DB_SRC_FILES db/plant_db db/plant_db_editor db/plant_db_editor_widgets db/plant_properties)
SET(RESOURCES_SRC_FILES resources/environment_manager resources/environment_illumination resources/environment_soil_humidity resources/environment_temp)
SET(GUI_SRC_FILES gui/main_window)
SET(WIDGETS_SRC_FILES gui/widgets/simulation_configuration_widget gui/widgets/plant_configuration_widget
gui/widgets/overview_widget gui/widgets/central_widget)
SET(DIALOGS_SRC_FILES gui/dialogs/start_config_dialog gui/dialogs/monthly_edit_dlg gui/dialogs/monthly_temp_edit_dlg gui/dialogs/monthly_illumination_edit_dlg
gui/dialogs/monthly_humidity_edit_dlg)
SET(RENDERING_SRC_FILES gui/rendering/renderer gui/rendering/render_manager gui/rendering/resource_visual_converters)
SET(DATA_HOLDERS_SRC_FILES data_holders/environment_spatial_hashmap
data_holders/plant_rendering_data data_holders/plant_rendering_data_container)
set(SIMULATOR_CORE_SRC_FILES simulator/core/simulation_configuration simulator/core/simulator_manager)
set(SIMULATOR_PLANTS_SRC_FILES simulator/plants/plant_factory simulator/plants/plants_storage simulator/plants/plant simulator/plants/growth_manager simulator/plants/constrainers)
SET(MATH_SRC_FILES math/linear_equation math/dice_roller)
SET(UTILS_SRC_FILES utils/utils utils/time_manager utils/debuger)

#link_directories("${CMAKE_SOURCE_DIR}/lib/statistical-analysis-tool/" "${CMAKE_SOURCE_DIR}/lib/ecodata-tracker/")
include_directories(${INCLUDE_DIRECTORIES})

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -std=c++11 -DGUI_MODE -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11 -DGUI_MODE -Wno-write-strings")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 -std=c++11 -DGUI_MODE -Wno-write-strings")

#SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)
#SET(CMAKE_BUILD_TYPE DEBUG)


add_executable(EcoSim main
${RESOURCES_SRC_FILES}
${WIDGETS_SRC_FILES}
${GUI_SRC_FILES}
${DATA_HOLDERS_SRC_FILES}
${DIALOGS_SRC_FILES}
${RENDERING_SRC_FILES}
${SIMULATOR_CORE_SRC_FILES}
${SIMULATOR_PLANTS_SRC_FILES}
${MATH_SRC_FILES}
${UTILS_SRC_FILES})

target_link_libraries(EcoSim ${LIBS})
# Use the Widgets module from Qt 5.

#INSTALL EXECUTABLE
install(TARGETS EcoSim
        RUNTIME DESTINATION bin
        CONFIGURATIONS RELWITHDEBINFO)
