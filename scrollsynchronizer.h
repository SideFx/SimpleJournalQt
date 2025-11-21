/////////////////////////////////////////////////////////////////////////////
// Name:        scrollsynchronizer.h
// Purpose:     Synchronize scrolling of MD editor with MD viewer (header)
// Author:      Jan Buchholz
// Created:     2025-11-20
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QSplitter>
#include <QTextEdit>

class ScrollSynchronizer : public QObject {
    Q_OBJECT

public:
    explicit ScrollSynchronizer(QTextEdit* a, QTextEdit* b, QSplitter* parentSplitter);
    void setEnabled(bool enabled);

private:
    QTextEdit* m_editA;
    QTextEdit* m_editB;
    QSplitter* m_splitter;
    bool m_syncing;
    bool m_enabled;

    void sync(QTextEdit* source, QTextEdit* target);

private slots:
    void syncBtoA() { sync(m_editB, m_editA); }
};


