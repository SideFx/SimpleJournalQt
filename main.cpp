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

int main(int argc, char *argv[]) {
    QString i18nPath = "";
    bool load = false;
    QApplication a(argc, argv);
#if defined (Q_OS_WIN)
    i18nPath = QCoreApplication::applicationDirPath() + "/translations/";
    a.setStyle(QStyleFactory::create("fusion"));
#elif defined (Q_OS_MAC)
    i18nPath = QCoreApplication::applicationDirPath() + "/../Resources/i18n/";
    a.setStyle(QStyleFactory::create("macos"));
#endif
    QTranslator translator;
    if (QLocale::system().name().startsWith("de_", Qt::CaseSensitive)) {
        load = translator.load(i18nPath + "qtbase_de.qm");
        load = load && translator.load(i18nPath + "SimpleJournalQt_de.qm");
    } else {
        load = load && translator.load(i18nPath + "qtbase_en.qm");
    }
    if (load) a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
