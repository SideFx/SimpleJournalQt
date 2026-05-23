/////////////////////////////////////////////////////////////////////////////
// Name:        io.cpp
// Purpose:     File load/save methods
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#include "io.h"
#include <QDir>

QString Io::save(QString fileName, QByteArray payload) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return ERR_FILEWRITE;
    file.write(payload);
    file.close();
    return {};
}

QString Io::load(QString fileName, QByteArray *payload) {
    payload->clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return ERR_FILEREAD;
    *payload = file.readAll();
    file.close();
    return {};
}
