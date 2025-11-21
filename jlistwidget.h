/////////////////////////////////////////////////////////////////////////////
// Name:        jlistwidget.h
// Purpose:     QListWidget subclassed (header)
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QListWidget>
#include <QObject>
#include <QDropEvent>

class JListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit JListWidget(QWidget *parent = nullptr);
    ~JListWidget();

protected:
    void dropEvent(QDropEvent *e) override;

signals:
    void dropEventAccepted();
};

