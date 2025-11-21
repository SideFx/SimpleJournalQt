/////////////////////////////////////////////////////////////////////////////
// Name:        jlistwidget.cpp
// Purpose:     QListWidget subclassed
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#include "jlistwidget.h"

JListWidget::JListWidget(QWidget *parent) : QListWidget(parent) {
}

JListWidget::~JListWidget() {
}

void JListWidget::dropEvent(QDropEvent *event) {
    QListView::dropEvent(event);
    if (event->isAccepted()) {
        emit dropEventAccepted();
    }
}
