/////////////////////////////////////////////////////////////////////////////
// Name:        io.h
// Purpose:     File load/save methods (header)
// Author:      Jan Buchholz
// Created:     2025-11-19
// Changed:     2026-11-06
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QString>
#include <QObject>
#include <QByteArray>

class AppIo {

public:
    AppIo() = default;
    ~AppIo() = default;
    QString save(QString fileName, QByteArray payload);
    QString load(QString fileName, QByteArray *payload);

private:
    QString const ERR_FILEREAD = QObject::tr("Error reading file.");
    QString const ERR_FILEWRITE = QObject::tr("Error writing file.");

};

