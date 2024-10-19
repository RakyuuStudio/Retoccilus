cmake_minimum_required(VERSION 3.21)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "E:/QtInstall/6.7.2/msvc2019_64") # < Absolute path, make sure to change when you want to build.

# NOTE: These paths that you need to change before you can build the project, or the dependencies
# won't be included correctly.
list(APPEND CMAKE_PREFIX_PATH "C:/Users/ryan_/.vcpkg-clion/vcpkg/installed/x64-windows")

include(C:/Users/ryan_/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake)

macro(find_package_general mode)
    foreach (pkg ${ARGN})
        find_package(${pkg} ${mode} REQUIRED)
    endforeach ()
endmacro()

find_package_general(CONFIG Qt6Core Qt6Widgets Qt6Gui)
find_package_general(CONFIG assimp glfw3 glm)
find_package(Vulkan REQUIRED)

if (Qt6_FOUND)
    message(STATUS "> Build: Qt6 Installation found.")
elseif (NOT Qt6_FOUND)
    message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed")
endif ()

include_directories(
)

set(Module_SceneWidget_Engine3D_Sources
    Source/Engine3D/SceneWidget/SceneWidget.h
)

# Target: Retoccilus all
add_executable(Retoccilus main.cpp
        # 3D Part
        Source/Engine3D/SceneWidget/SceneWidget.h
        Source/Engine3D/SceneWidget/SceneWidget.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui

        assimp::assimp
        glfw
        glm::glm
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII) # This is so fucking important, if not it will have lots of errors with QString
