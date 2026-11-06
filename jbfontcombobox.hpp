/////////////////////////////////////////////////////////////////////////////
// Name:        jbfontcombobox.hpp
// Purpose:     Derived from QFontComboBox
// Author:      Jan Buchholz
// Created:     2026-11-03
// Changed:     2026-11-05
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QFontComboBox>
#include <QAbstractItemView>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QFontMetrics>

class JBFontComboBox : public QFontComboBox {
    Q_OBJECT

public:
    explicit JBFontComboBox(QWidget *parent = nullptr) : QFontComboBox(parent), m_itemFontSize(-1) {}
    // ---set font size for selected font and font list---
    void setItemFontSize(int pointSize) {
        m_itemFontSize = pointSize;
        QFont f = this->font();
        if (pointSize > 0) {
            f.setPointSize(pointSize);
        }
        this->setFont(f);
        // ---delegate for Dropdown menu---
        if (pointSize > 0) {
            view()->setItemDelegate(new FontSizeDelegate(pointSize, this));
        } else {
            view()->setItemDelegate(nullptr);
        }
    }

    int itemFontSize() const {
        return m_itemFontSize;
    }

private:
    int m_itemFontSize;

    // ---internal delegate---
    class FontSizeDelegate : public QStyledItemDelegate {
        public:
            FontSizeDelegate(int pointSize, QObject *parent = nullptr)
                : QStyledItemDelegate(parent), m_pointSize(pointSize) {}
            void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override {
                QStyledItemDelegate::initStyleOption(option, index);
                QString fontName = index.data(Qt::DisplayRole).toString();
                if (!fontName.isEmpty()) {
                    option->font.setFamily(fontName);
                }
                option->font.setPointSize(m_pointSize);
            }
            // ---single line in list---
            QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
                QSize size = QStyledItemDelegate::sizeHint(option, index);
                QFontMetrics fm(option.font);
                size.setHeight(fm.height() + 6); // Font height + 6px buffer
                return size;
            }
        private:
            int m_pointSize;
    };
};
