# Welcome to the Source Code of Retoccilus. You can start learn from this.
cmake_minimum_required(VERSION 3.21)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PREFIX_PATH "E:/QtInstall/6.7.2/msvc2019_64") # < Absolute path, make sure to change when you want to build.

# NOTE: These paths that you need to change before you can build the project, or the dependencies
# won't be included correctly.
# It's a magic problem, I deploy Vcpkg with CLion Documentation, and it can't find the package, idk why :(
# So I use Vcpkg as a package downloader and deployer, and I include it as normal package.
# If you can solve the problem, you can comment the code below and do the Vcpkg documentation want you to do.
list(APPEND CMAKE_PREFIX_PATH "C:/Users/ryan_/.vcpkg-clion/vcpkg/packages/assimp_x64-windows/share/assimp")
list(APPEND CMAKE_PREFIX_PATH "C:/Users/ryan_/.vcpkg-clion/vcpkg/packages/pugixml_x64-windows/share/pugixml")

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


if (Qt6_FOUND)
    message(STATUS "> Build: Qt6 Installation found.")
elseif (NOT Qt6_FOUND)
    message(FATAL_ERROR "> Build: Qt6 Installation wasn't found. Build Failed'")
endif ()

include_directories(
#        Source/Engine3D
)

# Target: Retoccilus all
add_executable(Retoccilus main.cpp
        # 3D Part
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        assimp::assimp
)

remove_definitions(-DQT_NO_CAST_FROM_ASCII) # This is so fucking important, if not it will have lots of errors with QString
