/////////////////////////////////////////////////////////////////////////////
// Name:        io.h
// Purpose:     File load/save methods (header)
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QString>
#include <QObject>
#include <QByteArray>

class Io {

public:
    Io() = default;
    ~Io() = default;
    QString save(QString fileName, QByteArray payload);
    QString load(QString fileName, QByteArray *payload);

private:
    QString const ERR_FILEREAD = QObject::tr("Error reading file.");
    QString const ERR_FILEWRITE = QObject::tr("Error writing file.");

};

