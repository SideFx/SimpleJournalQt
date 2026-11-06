/////////////////////////////////////////////////////////////////////////////
// Name:        io.cpp
// Purpose:     File load/save methods
// Author:      Jan Buchholz
// Created:     2025-11-19
// Changed:     2026-11-06
/////////////////////////////////////////////////////////////////////////////

#include "appio.h"
#include <QDir>

QString AppIo::save(QString fileName, QByteArray payload) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return ERR_FILEWRITE;
    file.write(payload);
    file.close();
    return {};
}

QString AppIo::load(QString fileName, QByteArray *payload) {
    payload->clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return ERR_FILEREAD;
    *payload = file.readAll();
    file.close();
    return {};
}
