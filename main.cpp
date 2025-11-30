/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     Main
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#if defined (Q_OS_WIN)
    a.setStyle(QStyleFactory::create("fusion"));
#elif defined (Q_OS_MAC)
    a.setStyle(QStyleFactory::create("macos"));
#endif
    QString i18nPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    QTranslator translator;
    if (translator.load(i18nPath + "qt_en.qm")) a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
