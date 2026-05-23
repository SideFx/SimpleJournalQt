/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     Main
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-04-05
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#if defined (Q_OS_WIN)
    a.setStyle(QStyleFactory::create("fusion"));
#elif defined (Q_OS_MAC)
    a.setStyle(QStyleFactory::create("macos"));
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
