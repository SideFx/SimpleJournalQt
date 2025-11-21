/////////////////////////////////////////////////////////////////////////////
// Name:        iconlist.h
// Purpose:     List of SVG icons for note listbox (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QIcon>

class IconList : public QObject {
    Q_OBJECT

public:
    explicit IconList(QObject *parent = nullptr);
    const QString m_icon_prefix = ":/list/assets/";
    const QStringList m_listIcons = {{"list_Article.svg"},
                                     {"list_Bookmarks.svg"},
                                     {"list_Calendar.svg"},
                                     {"list_Contacts.svg"},
                                     {"list_DataTable.svg"},
                                     {"list_Drafts.svg"},
                                     {"list_Favorite.svg"},
                                     {"list_Folder.svg"},
                                     {"list_Label.svg"},
                                     {"list_Library.svg"},
                                     {"list_Objects.svg"},
                                     {"list_Pin.svg"},
                                     {"list_Smiley.svg"},
                                     {"list_Support.svg"},
                                     {"list_Work.svg"}
    };
    QIcon getIconAtIndex(int index);
};

