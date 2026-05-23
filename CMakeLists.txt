cmake_minimum_required(VERSION 3.16)

project(SimpleJournalQt VERSION 0.1 LANGUAGES CXX)

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

    qt_add_executable(SimpleJournalQt
        WIN32 MACOSX_BUNDLE
        MANUAL_FINALIZATION
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        stylesheets.h
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
        io.h io.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        scrollsynchronizer.h scrollsynchronizer.cpp
        synchelper.cpp synchelper.h
        imagehandler.cpp imagehandler.h
    )
endif()

target_link_libraries(SimpleJournalQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SimpleJournalQt PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Svg)
target_link_libraries(SimpleJournalQt PRIVATE md4c md4c-html)

set_target_properties(SimpleJournalQt PROPERTIES
    ${BUNDLE_ID_OPTION}
#    UPDATE_TRANSLATIONS .
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SimpleJournalQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(APPLE)
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
    find_program(MACDEPLOYQT macdeployqt6 HINTS "${QT_BIN_DIR}")
    add_custom_command(TARGET SimpleJournalQt POST_BUILD
        COMMAND ${MACDEPLOYQT} "$<TARGET_BUNDLE_DIR:SimpleJournalQt>" -verbose=2
    )
    add_custom_command(TARGET SimpleJournalQt POST_BUILD
        COMMAND codesign --force --deep --sign - --timestamp=none "$<TARGET_BUNDLE_DIR:SimpleJournalQt>"
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SimpleJournalQt)
endif()

