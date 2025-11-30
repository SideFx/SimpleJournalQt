/////////////////////////////////////////////////////////////////////////////
// Name:        jblistwidget.cpp
// Purpose:     QListWidget subclassed
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#include "jblistwidget.h"

JBListWidget::JBListWidget(QWidget *parent) : QListWidget(parent) {
}

JBListWidget::~JBListWidget() {
}

void JBListWidget::dropEvent(QDropEvent *event) {
    QListView::dropEvent(event);
    if (event->isAccepted()) {
        emit dropEventAccepted();
    }
}
