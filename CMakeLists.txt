cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "SimpleJournalQt")
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_subdirectory("${CMAKE_SOURCE_DIR}/md4c")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Svg)
find_package(Qt6 REQUIRED COMPONENTS Core)

if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON "${CMAKE_SOURCE_DIR}/app.icns")
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

    qt_add_executable(${PROJECT_NAME}
        WIN32 MACOSX_BUNDLE
        MANUAL_FINALIZATION
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        jbstyles.hpp
        constants.h
        jbpreferences.cpp jbpreferences.h
        resources.qrc
        dialogs.h dialogs.cpp
        uilogic.h uilogic.cpp
        jblistwidget.h jblistwidget.cpp
        listfontdialog.h listfontdialog.cpp listfontdialog.ui
        listitemdialog.h listitemdialog.cpp listitemdialog.ui
        iconlist.h iconlist.cpp
        app.icns app.ico appicon.rc
        appicon.rc
        appio.h appio.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        scrollsynchronizer.h scrollsynchronizer.cpp
        synchelper.cpp synchelper.h
        imagehandler.cpp imagehandler.h
        jbfontcombobox.hpp
        Info.plist.in
    )
endif()

#include <jbfontcombobox.hpp> // <-- brackets, because of widget promotion!
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Svg)
target_link_libraries(${PROJECT_NAME} PRIVATE md4c md4c-html)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- macOS Deployment ----
if(APPLE)
    # 1. Create Debug-Entitlements file in the build folder
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/debug_entitlements.plist
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://apple.com\">\n"
            "<plist version=\"1.0\">\n"
            "<dict>\n"
            "    <key>com.apple.security.get-task-allow</key>\n"
            "    <true/>\n"
            "</dict>\n"
            "</plist>\n"
    )
    # 2. Deploy
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
    find_program(MACDEPLOYQT macdeployqt6 HINTS "${QT_BIN_DIR}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${MACDEPLOYQT} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>" -verbose=2
    )
    # 3. Sign using two different steps
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND codesign --force --deep --sign - --timestamp=none "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
        COMMAND codesign --force --sign - --timestamp=none --entitlements
            ${CMAKE_CURRENT_BINARY_DIR}/debug_entitlements.plist "$<TARGET_FILE:${PROJECT_NAME}>"
        #COMMAND codesign --force --deep --sign - --timestamp=none "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>"
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

