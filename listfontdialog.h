/////////////////////////////////////////////////////////////////////////////
// Name:        listfontdialog.h
// Purpose:     Dialog for note list fonts & icons (header)
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QDialog>

namespace Ui {
class ListFontDialog;
}

class ListFontDialog : public QDialog {
    Q_OBJECT

public:
    explicit ListFontDialog(QWidget *parent = nullptr);
    ~ListFontDialog();
    QDialog::DialogCode Execute(QFont *font, int *size);

private:
    Ui::ListFontDialog *ui;
    QFont m_selectedFont;
    int m_selectedFontSize;
    int m_selectedIconSize;
    const QStringList m_fontSizeList = {"8", "9", "10", "11", "12", "13", "14", "16", "18"};
    const QStringList m_iconSizeList = {"12", "14", "16", "18", "20", "22", "24"};

private slots:
    void onCurrentFontChanged(const QFont);
    void onCurrentTextChanged_Font(const QString);
    void onCurrentTextChanged_Icon(const QString);

};

