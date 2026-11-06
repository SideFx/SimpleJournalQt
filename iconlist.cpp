/////////////////////////////////////////////////////////////////////////////
// Name:        iconlist.cpp
// Purpose:     List of SVG icons for note listbox
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-11-06
/////////////////////////////////////////////////////////////////////////////

#include "iconlist.h"

IconList::IconList(QObject *parent) : QObject{parent} {
}

QIcon IconList::getIconAtIndex(int index) {
    if (index < m_listIcons.count())
    return QIcon(m_icon_prefix + m_listIcons.at(index));
    return QIcon();
}

QString IconList::getIconPathAtIndex(int index) {
    if (index < m_listIcons.count()) {
        return m_icon_prefix + m_listIcons.at(index);
    }
    return "";
}
