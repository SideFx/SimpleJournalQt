/////////////////////////////////////////////////////////////////////////////
// Name:        imahehandler.h
// Purpose:     Load pictures (gif, png, jpeg, webp, svg) (header)
// Author:      Jan Buchholz
// Created:     2025-11-25
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QImage>
#include <QUrl>
#include <QNetworkAccessManager>

class ImageHandler : public QObject {
    Q_OBJECT

public:
    explicit ImageHandler(QObject *parent = nullptr);
    ~ImageHandler();
    QImage getFromCache(QString name, QString workDir);
    void invalidateCache();

private:
    QImage readFromURL(QUrl url);
    QImage readFromPath(QString path, QString workDir);
    QImage readFromAnywhere(QString path, QString workDir);
    QImage getFromBuffer(QByteArray &buffer);

    QNetworkAccessManager *m_manager;
    std::map<QString, QImage> m_cache;
};

