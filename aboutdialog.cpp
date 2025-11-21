/////////////////////////////////////////////////////////////////////////////
// Name:        aboutdialog.cpp
// Purpose:     Create About dialog
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#include <QPushButton>
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Close"));
    setFixedSize(this->geometry().width(),this->geometry().height());
    setWindowTitle(TITLE_ABOUT);
    ui->lbl_text1->setText(TxtAboutJournal);
}

AboutDialog::~AboutDialog() {
    delete ui;
}
