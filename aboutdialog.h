/////////////////////////////////////////////////////////////////////////////
// Name:        aboutdialog.h
// Purpose:     About dialog header
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

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
    QString TITLE_ABOUT = tr("About SimpleJournalQt");
    QString TxtAboutJournal = tr("SimpleJournalQt (w) 2025 Jan Buchholz\n***\n") +
    tr("Created with Qt Community Edition v") + qVersion() + tr(" (https://www.qt.io).") +
    tr("\n***\nThanks to Martin Mitáš (mity)\n") +
    tr("and all contributors to the \"md4c\" library:\nhttps://github.com/mity/md4c");
};


