/////////////////////////////////////////////////////////////////////////////
// Name:        imahehandler.cpp
// Purpose:     Load pictures (gif, png, jpeg, webp, svg)
// Author:      Jan Buchholz
// Created:     2025-11-25
/////////////////////////////////////////////////////////////////////////////

#include "imagehandler.h"
#include <QNetworkRequestFactory>
#include <QNetworkRequest>
#include <QImageReader>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <qnetworkreply.h>

ImageHandler::ImageHandler(QObject *parent) : QObject{parent} {
    m_manager = new QNetworkAccessManager();
    invalidateCache();
}

ImageHandler::~ImageHandler() {
    delete m_manager;
}

QImage ImageHandler::getFromCache(QString name, QString workDir) {
    if (const auto it = m_cache.find(name); it != m_cache.end()) {
        return it->second;
    }
    QImage image = readFromAnywhere(name, workDir);
    m_cache.insert({name, image});
    return image;
}

void ImageHandler::invalidateCache() {
    m_cache.clear();
}

QImage ImageHandler::readFromURL(QUrl url) {
    QImage image;
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "MarkdownEditorQt/1.0");
    QNetworkReply* reply = m_manager->get(req);
    QEventLoop loop;
    // finished
    QObject::connect(reply, &QNetworkReply::finished, &loop, [&]() {
        loop.quit();
    });
    // timeout
    auto* timer = new QTimer(reply);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, &loop, [&]() {
        loop.quit();
    });
    timer->start(5000);
    loop.exec();
    const bool timedOut = !reply->isFinished();
    if (timedOut) reply->abort();
    if (!timedOut && reply->error() == QNetworkReply::NoError) {
        QByteArray buffer = reply->readAll();
        image = getFromBuffer(buffer);
    } else {
        qWarning() << "Network error:" << (timedOut ? "timeout" : reply->errorString());
    }
    reply->deleteLater();
    return image;
}

QImage ImageHandler::readFromPath(QString path, QString workDir) {
    QString absolutePath;
    if (workDir.isEmpty()) workDir = QDir::currentPath();
    QFileInfo fi(path);
    if (fi.isAbsolute()) absolutePath = path;
    else {
        QFileInfo imgInfo(workDir, path);
        absolutePath = imgInfo.absoluteFilePath();
    }
    QFile file(absolutePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray buffer = file.readAll();
        file.close();
        return getFromBuffer(buffer);
    }
    return QImage{};
}

QImage ImageHandler::readFromAnywhere(QString path, QString workDir) {
    QUrl url(path);
    if (url.isValid() && !url.scheme().isEmpty()) {
        return readFromURL(url);
    } else {
        return readFromPath(path, workDir);
    }
}

QImage ImageHandler::getFromBuffer(QByteArray &buffer) {
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForData(buffer);
    if (mime.name() == "image/png" || mime.name() == "image/jpeg" ||
        mime.name() == "image/gif" || mime.name() == "image/webp") {
        QImage image;
        if (image.loadFromData(buffer)) return image;
    } else if (mime.name() == "image/svg+xml") {
        QSvgRenderer renderer(buffer);
        if (renderer.isValid()) {
            QSize size = renderer.defaultSize();
            QImage image(size, QImage::Format_ARGB32);
            image.fill(Qt::transparent);
            QPainter painter(&image);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::TextAntialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            renderer.render(&painter);
            return image;
        }
    }
    return QImage{};
}
