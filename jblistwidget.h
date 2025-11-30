/////////////////////////////////////////////////////////////////////////////
// Name:        jblistwidget.h
// Purpose:     QListWidget subclassed (header)
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QListWidget>
#include <QObject>
#include <QDropEvent>

class JBListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit JBListWidget(QWidget *parent = nullptr);
    ~JBListWidget();

protected:
    void dropEvent(QDropEvent *e) override;

signals:
    void dropEventAccepted();
};

