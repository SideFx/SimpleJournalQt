/////////////////////////////////////////////////////////////////////////////
// Name:        aboutdialog.h
// Purpose:     About dialog header
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-05-22
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>
#include "constants.h"

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    QString TxtAboutJournal = QString(tr("SimpleJournalQt v") + APPVERSION +
    tr(" (w) 2026 Jan Buchholz\n\u2022\u2022\u2022\n")) +
    tr("Created with Qt Community Edition v") + qVersion() + tr(" (https://www.qt.io).") +
    tr("\n\u2022\u2022\u2022\nThanks to Martin Mitáš (mity)\n") +
    tr("and all contributors to the \"md4c\" library:\nhttps://github.com/mity/md4c");
};


