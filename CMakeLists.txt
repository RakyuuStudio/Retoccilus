# Welcome to the Source Code of Retoccilus. You can start learn from this.
cmake_minimum_required(VERSION 3.15)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH "E:/QtInstall/6.7.2/msvc2019_64") # < Absolute path, make sure to change when you want to build.

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)

find_package(Vulkan REQUIRED)

if(NOT Qt6_FOUND)
	message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed'")
endif()

add_subdirectory(Source/Engine/3D-Support)

set(FBXLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty_lib/fbxsdk/libfbxsdk.lib")
set(XML2LIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty_lib/fbxsdk/libxml2.lib")
set(ALEMBICLIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty_lib/fbxsdk/alembic.lib")
set(GLFW3LIB_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty_lib/glfw/glfw3.lib")

include_directories(
		thirdparty_lib/glfw/include
		thirdparty_lib/vulkan/include
		thirdparty_lib/fbxsdk/include
)

add_executable(${PROJECT_NAME} main.cpp
#		Source/Engine/3D-Support/Model-Importer/include/fbx-importer.h
#		Source/Engine/3D-Support/Model-Importer/src/fbx-importer.cpp
#		Source/Engine/3D-Support/PublicAPI/Public3DApi.cpp
#		Source/Engine/3D-Support/PublicAPI/Public3DApi.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
		Qt6::Core
		Qt6::Widgets
		Qt6::Gui
		Vulkan::Vulkan
		${FBXLIB_LIBRARY}
		${XML2LIB_LIBRARY}
		${ALEMBICLIB_LIBRARY}
		${GLFW3LIB_LIBRARY}
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)