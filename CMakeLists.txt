# Welcome to the Source Code of Retoccilus. You can start learn from this.
cmake_minimum_required(VERSION 3.21)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "E:/QtInstall/6.7.2/msvc2019_64") # < Absolute path, make sure to change when you want to build.

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)

if(Qt6_FOUND)
	message(STATUS "> Build: Qt6 Installation found.")
elseif(NOT Qt6_FOUND)
	message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed'")
endif()

include_directories(

)

# Target: Retoccilus all
add_executable(Retoccilus main.cpp
	# 3D Part
		Source/Engine3D/ModelImporter/FBX-Importer/fbx-importer.cpp
		Source/Engine3D/ModelImporter/FBX-Importer/fbx-importer.h
		Source/Engine3D/ModelImporter/Utility/RetoMIString/RetoMIString.cpp
		Source/Engine3D/ModelImporter/Utility/RetoMIString/RetoMIString.h
        Source/Engine3D/ModelImporter/Utility/Container/Vector2D.inl
        Source/Engine3D/ModelImporter/Utility/Container/Vector2D.h
        Source/Engine3D/ModelImporter/Utility/Container/Vector3D.inl
        Source/Engine3D/ModelImporter/Utility/Container/Vector3D.h
        Source/Engine3D/ModelImporter/Utility/Container/Matrix3x3.inl
        Source/Engine3D/ModelImporter/Utility/Container/Matrix3x3.h
        Source/Engine3D/ModelImporter/Utility/Container/Matrix4x4.cpp
        Source/Engine3D/ModelImporter/Utility/Container/Matrix4x4.h
        Source/Engine3D/ModelImporter/Utility/Container/Quaternion.cpp
        Source/Engine3D/ModelImporter/Utility/Container/Quaternion.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
		Qt6::Core
		Qt6::Widgets
		Qt6::Gui
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)
