/////////////////////////////////////////////////////////////////////////////
// Name:        scrollsynchronizer.cpp
// Purpose:     Synchronize scrolling of MD editor with MD viewer
// Author:      Jan Buchholz
// Created:     2025-11-20
/////////////////////////////////////////////////////////////////////////////

#include "scrollsynchronizer.h"
#include <QScrollBar>

ScrollSynchronizer::ScrollSynchronizer(QTextEdit* a, QTextEdit* b, QSplitter* parentSplitter)
    : QObject(parentSplitter), m_editA(a), m_editB(b), m_splitter(parentSplitter), m_syncing(false) {
    connect(m_editB->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &ScrollSynchronizer::syncBtoA);
}

void ScrollSynchronizer::setEnabled(bool enabled) {
    m_enabled = enabled;
}

void ScrollSynchronizer::sync(QTextEdit* source, QTextEdit* target) {
    if (m_syncing || !m_enabled) return;
    QList<int> sizes = m_splitter->sizes();
    if (sizes[1] == 0) return; // editor visible?
    m_syncing = true;
    QScrollBar* srcBar = source->verticalScrollBar();
    QScrollBar* tgtBar = target->verticalScrollBar();
    int srcRange = srcBar->maximum() - srcBar->minimum();
    int tgtRange = tgtBar->maximum() - tgtBar->minimum();
    if (srcRange > 0 && tgtRange > 0) {
        double ratio = double(srcBar->value() - srcBar->minimum()) / srcRange;
        int newValue = tgtBar->minimum() + ratio * tgtRange;
        if (std::abs(tgtBar->value() - newValue) > 1) tgtBar->setValue(newValue);
    }
    m_syncing = false;
}
