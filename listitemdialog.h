/////////////////////////////////////////////////////////////////////////////
// Name:        listitemdialog.h
// Purpose:     Dialog to set note properties (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

namespace Ui {
class ListItemDialog;
}

class ListItemDialog : public QDialog {
    Q_OBJECT

public:
    explicit ListItemDialog(QWidget *parent = nullptr);
    ~ListItemDialog();
    enum mode {create, edit};
    QDialog::DialogCode Execute(mode m, QString *subject, int *icon);

private:
    Ui::ListItemDialog *ui;
    void setUpIconBox();

private slots:
    void onTextChanged();

};

