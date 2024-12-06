cmake_minimum_required(VERSION 3.20)
project(Retoccilus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "D:/Programs/Qt/6.8.0/mingw_64") # < This is my Absolute Path that points to my Qt Installation.
                                                       # < You should change this to make sure you can load the project.

macro(find_package_general mode)
    foreach (pkg ${ARGN})
        find_package(${pkg} ${mode} REQUIRED)
    endforeach ()
endmacro()

find_package_general(CONFIG Qt6Core Qt6Widgets Qt6Gui)
find_package(Vulkan REQUIRED)

if (Qt6_FOUND)
    message(STATUS "Build: Qt6 Installation found.")
elseif (NOT Qt6_FOUND)
    message(FATAL_ERROR "Build: Qt6 Installation wasn't found. Build Failed")
endif ()
    
set(PROJECT_RESOURCES Resources/Resources.qrc)
qt_add_resources(RESOURCE_SRCS ${PROJECT_RESOURCES})

add_executable(Retoccilus
    main.cpp
    
    src/Core/FileOperation/AbstractFileOperation.hpp
    src/Core/FileOperation/FileOperation.hpp
    src/Core/AbstractClasses/SceneWidget/AbstractSceneWidget.cpp
    src/Core/AbstractClasses/SceneWidget/AbstractSceneWidget.hpp
    src/Editor/SceneWidget/SceneWidget2D.hpp
    src/Editor/SceneWidget/SceneWidget2D.cpp

    ${RESOURCE_SRCS}
)

add_subdirectory(ThirdParty/glm/glm)
add_subdirectory(ThirdParty/glfw)

# add_dependencies(Retoccilus Resources/Shaders)

include_directories(
    src/
)

target_link_libraries(Retoccilus PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui

    Vulkan::Vulkan
    glfw
    glm
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)