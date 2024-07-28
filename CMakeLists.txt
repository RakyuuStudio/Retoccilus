cmake_minimum_required(VERSION 3.15)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)

if(NOT Qt6_FOUND)
	message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed'")
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)