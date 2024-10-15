# Welcome to the Source Code of Retoccilus. You can start learn from this.
cmake_minimum_required(VERSION 3.21)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "E:/QtInstall/6.7.2/msvc2019_64") # < Absolute path, make sure to change when you want to build.

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)

file(GLOB ModelImporterModule_Utility_Container
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/Container/*.h"
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/Container/*.inl"
)

file(GLOB ModelImporterModule_Utility_ModelData
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/ModelData/*.h"
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/ModelData/*.cpp"
)

file(GLOB ModelImporterModule_Utility_Tools
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/Tools/*.h"
        "${CMAKE_SOURCE_DIR}/Source/Engine3D/ModelImporter/Utility/Tools/*.cpp"
)

if (Qt6_FOUND)
    message(STATUS "> Build: Qt6 Installation found.")
elseif (NOT Qt6_FOUND)
    message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed'")
endif ()

include_directories(
        Source/Engine3D
)

# Target: Retoccilus all
add_executable(Retoccilus main.cpp
        # 3D Part
        ${ModelImporterModule_Utility_Container}
        ${ModelImporterModule_Utility_ModelData}
        ${ModelImporterModule_Utility_Tools}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII) # This is so fucking important, if not it will have lots of errors with QString
