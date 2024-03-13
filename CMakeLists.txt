cmake_minimum_required(VERSION 3.26)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "E:/Qt/6.6.2/mingw_64/lib/cmake")
set(SARibbonBar_DIR "F:/Retoccilus/ThirdPartyLibraries/SARibbon/bin_qt6.6.2_GNU_x64/lib/cmake/SARibbonBar")
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        Qml
        Quick
        Core5Compat
        REQUIRED)

find_package(SARibbonBar)

qt_add_resources(RESOURCES Resources/Resources.qrc)

add_executable(Retoccilus main.cpp
        Sources/MainWindow/rmainwindow.h
        Sources/MainWindow/rmainwindow.cpp
        Sources/RCodeEditor/RCodeEditor.h
        Sources/RCodeEditor/RCodeEditor.cpp
        ${RESOURCES}
        Sources/RCodeEditor/RSyntaxHighlighter.h
        Sources/RCodeEditor/RCFamilyHighlighter.h
        Sources/RCodeEditor/RCFamilyHighlighter.h
        Sources/RCodeEditor/RHighlightRule.h
        Sources/RCodeEditor/RHighlightRule.h
        Sources/RCodeEditor/RStyleSyntaxHighlighter.h
        Sources/RCodeEditor/RSyntaxStyle.h
        Sources/RCodeEditor/RBorderTextProperty.h
        Sources/RCodeEditor/RSidebar.h
        Sources/RCodeEditor/RLanguage.h
        Sources/RCodeEditor/foldingHandler.h
        Sources/RCodeEditor/foldingHandler.cpp
        Parser/Cpp/Tokenizer.h
        Parser/utils/ParserUtils.h
        Sources/Global/utils.h
        Sources/Global/qt_lib_utils.h
        Sources/Global/charset_info.h
        Sources/Global/SimpleIni.h
)

include_directories(
        utils/
        Sources/RCodeEditor/
        Sources/
        Parser/
)

target_link_libraries(Retoccilus
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Xml
        Qt::Qml
        Qt::Quick
        Qt6::Core5Compat

        SARibbonBar
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
