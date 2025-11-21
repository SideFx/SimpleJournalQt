/////////////////////////////////////////////////////////////////////////////
// Name:        listitemdialog.cpp
// Purpose:     Dialog to set note properties (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#include "listitemdialog.h"
#include "ui_listitemdialog.h"
#include "iconlist.h"
#include <QPushButton>

ListItemDialog::ListItemDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ListItemDialog) {
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    setFixedSize(this->geometry().width(),this->geometry().height());
    setUpIconBox();
    connect(ui->edit_subject, &QLineEdit::textChanged, this, &ListItemDialog::onTextChanged);
}

ListItemDialog::~ListItemDialog() {
    delete ui;
}

QDialog::DialogCode ListItemDialog::Execute(mode m, QString *subject, int *icon) {
    if (m == edit) {
        ui->edit_subject->setText(*subject);
        if (*icon < ui->iconBox->count()) ui->iconBox->setCurrentIndex(*icon);
        setWindowTitle(tr("Edit note topic"));
    } else {
        ui->edit_subject->clear();
        ui->iconBox->setCurrentIndex(0);
        setWindowTitle(tr("Create new note"));
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    ui->edit_subject->setFocus();
    if (exec() == QDialog::Accepted) {
        *subject = ui->edit_subject->text();
        *icon = ui->iconBox->currentIndex();
        return QDialog::Accepted;
    }
    return QDialog::Rejected;
}

void ListItemDialog::setUpIconBox() {
    IconList *iconList = new IconList();
    for (int i = 0; i < iconList->m_listIcons.count(); i++) {
        ui->iconBox->addItem(iconList->getIconAtIndex(i), "", i);
    }
    delete iconList;
}

void ListItemDialog::onTextChanged() {
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->edit_subject->text().isEmpty());
}
