/////////////////////////////////////////////////////////////////////////////
// Name:        listfontdialog.cpp
// Purpose:     Dialog for note list fonts & icons
// Author:      Jan Buchholz
// Created:     2025-11-19
/////////////////////////////////////////////////////////////////////////////

#include <QPushButton>
#include "listfontdialog.h"
#include "ui_listfontdialog.h"

ListFontDialog::ListFontDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ListFontDialog) {
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    setFixedSize(this->geometry().width(),this->geometry().height());
    ui->fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    ui->fontComboBox->setWritingSystem(QFontDatabase::Latin);
    ui->fontSizeBox->addItems(m_fontSizeList);
    ui->iconSizeBox->addItems(m_iconSizeList);
    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this, &ListFontDialog::onCurrentFontChanged);
    connect(ui->fontSizeBox, &QComboBox::currentTextChanged, this, &ListFontDialog::onCurrentTextChanged_Font);
    connect(ui->iconSizeBox, &QComboBox::currentTextChanged, this, &ListFontDialog::onCurrentTextChanged_Icon);
}

ListFontDialog::~ListFontDialog() {
    delete ui;
}

QDialog::DialogCode ListFontDialog::Execute(QFont *font, int *size) {
    if (!font->family().isEmpty()) {
        m_selectedFont = *font;
        m_selectedFontSize = font->pointSize();
        m_selectedIconSize = *size;
        ui->fontComboBox->setCurrentFont(m_selectedFont);
        ui->fontSizeBox->setCurrentText(QString::number(m_selectedFontSize));
        ui->iconSizeBox->setCurrentText(QString::number(m_selectedIconSize));
    }
    if (exec() == QDialog::Accepted) {
        *font = m_selectedFont;
        font->setPointSize(m_selectedFontSize);
        *size = m_selectedIconSize;
        return QDialog::Accepted;
    }
    return QDialog::Rejected;
}

void ListFontDialog::onCurrentFontChanged(const QFont font) {
    if (!font.family().isEmpty()) {
        m_selectedFont = font;
        int s = ui->fontSizeBox->currentText().toInt();
        m_selectedFont.setPointSize(s);
    }
}

void ListFontDialog::onCurrentTextChanged_Font(const QString size) {
    m_selectedFontSize = size.toInt();
}

void ListFontDialog::onCurrentTextChanged_Icon(const QString size) {
    m_selectedIconSize = size.toInt();
}
