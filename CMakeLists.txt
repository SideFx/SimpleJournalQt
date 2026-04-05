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

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
add_subdirectory("${CMAKE_SOURCE_DIR}/md4c")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Svg)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(QM_FILES ${CMAKE_SOURCE_DIR}/translations/qt_en.qm)

if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON "${CMAKE_SOURCE_DIR}/app.icns")
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

    qt_add_executable(SimpleJournalQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        stylesheets.h
        constants.h
        jbpreferences.cpp jbpreferences.h
        assets/mainTB_Unlocked.svg assets/mainTB_Font.svg assets/mainTB_Open.svg assets/mainTB_Quit.svg assets/mainTB_Save.svg assets/mainTB_SaveAs.svg assets/treeTB_CollapseAll.svg assets/treeTB_CreateFolder.svg assets/treeTB_CreateNote.svg assets/treeTB_Delete.svg assets/treeTB_ExpandAll.svg
        assets/mainTB_Locked.svg
        resources.qrc
        assets/mainTB_New.svg
        assets/mainTB_Copy.svg assets/mainTB_Cut.svg assets/mainTB_Paste.svg assets/mainTB_Redo.svg assets/mainTB_SelectAll.svg assets/mainTB_Undo.svg
        assets/mainTB_Info.svg
        assets/mainTB_Settings.svg
        dialogs.h dialogs.cpp
        uilogic.h uilogic.cpp
        jblistwidget.h jblistwidget.cpp
        assets/mainTB_Quit_2.svg
        assets/list_Article.svg assets/list_Bookmarks.svg assets/list_Calendar.svg assets/list_Contacts.svg assets/list_DataTable.svg assets/list_Drafts.svg assets/list_Favorite.svg assets/list_Folder.svg assets/list_Label.svg assets/list_Library.svg assets/list_Objects.svg assets/list_Pin.svg assets/list_Smiley.svg assets/list_Support.svg assets/list_Work.svg
        listfontdialog.h listfontdialog.cpp listfontdialog.ui
        assets/mainTB_Edit.svg
        assets/mainTB_Edit_2.svg
        assets/mainTB_Key.svg
        listitemdialog.h listitemdialog.cpp listitemdialog.ui
        iconlist.h iconlist.cpp
        app.icns app.ico appicon.rc
        appicon.rc
        io.h io.cpp
        assets/Journal.png
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        assets/mainTB_Export.svg
        scrollsynchronizer.h scrollsynchronizer.cpp
        LICENSE.md README.md
        images/img01.png images/img02.png images/img03.png images/img04.png images/img05.png
        imagehandler.cpp imagehandler.h synchelper.cpp synchelper.h
        assets/mainTB_FormatBold.svg assets/mainTB_FormatCode.svg assets/mainTB_FormatHighlight.svg assets/mainTB_FormatItalic.svg assets/mainTB_FormatStrikethrough.svg assets/mainTB_FormatUnderlined.svg
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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SimpleJournalQt)
endif()

