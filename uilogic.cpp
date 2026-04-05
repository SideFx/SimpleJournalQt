/////////////////////////////////////////////////////////////////////////////
// Name:        uilogic.cpp
// Purpose:     The main window (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-04-05
/////////////////////////////////////////////////////////////////////////////

#include "uilogic.h"
#include "listfontdialog.h"
#include "listitemdialog.h"
#include "iconlist.h"
#include <QJsonArray>
#include <QJsonDocument>

UILogic::UILogic(QObject *parent) : QObject{parent} {
    createListWidget();
    createMdViewer();
    createMdEditor();
    mc_synchelper = new SyncHelper(m_mdEditor, m_mdViewer);
    resetAll();
    connect(m_listWidget, &JBListWidget::currentItemChanged, this, &UILogic::onCurrentItemChanged);
    connect(m_listWidget, &JBListWidget::dropEventAccepted, this, &UILogic::onJBListDropEvent);
    connect(m_mdEditor, &QTextEdit::textChanged, this, &UILogic::onTextChanged);
}

UILogic::~UILogic() {
    delete m_listWidget;
    delete mc_synchelper;
    delete m_mdViewer;
    delete m_mdEditor;
}

void UILogic::startUp(const QByteArray ba, QString workDir) {
    resetAll();
    if (ba.isEmpty()) return;
    mc_synchelper->setDocumentPath(workDir);
    jsonToData(ba);
    IconList *iconList = new IconList();
    for (auto &j : m_data) {
        new QListWidgetItem(iconList->getIconAtIndex(j.icon), j.subject, m_listWidget, j.id);
        if (m_id < j.id) m_id = j.id;
    }
    delete iconList;
    if (m_listWidget->count() > 0) {
        QListWidgetItem *item = m_listWidget->item(0);
        m_listWidget->setCurrentItem(item);
    }
    m_mdViewer->setFocus();
}

void UILogic::resetAll() {
    m_listWidget->clear();
    m_mdViewer->clear();
    m_mdEditor->clear();
    m_data.clear();
    mc_synchelper->invalidateImageCache();
    m_id = QListWidgetItem::UserType;
}

void UILogic::createListWidget(){
    m_listWidget = new JBListWidget();
    m_listWidget->setMinimumWidth(150);
    m_listWidget->setViewMode(QListView::ListMode);
    m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_listWidget->setSortingEnabled(false);
    m_listWidget->setDragEnabled(true);
    m_listWidget->setAcceptDrops(true);
    m_listWidget->setDropIndicatorShown(true);
    m_listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_listWidget->setDragDropOverwriteMode(false);
    m_listWidget->setDefaultDropAction(Qt::MoveAction);
}

void UILogic::createMdViewer() {
    m_mdViewer = new QTextBrowser();
    m_mdViewer->setWordWrapMode(QTextOption::WordWrap);
    m_mdViewer->setLineWrapMode(QTextEdit::WidgetWidth);
    m_mdViewer->setAcceptRichText(false);
    m_mdViewer->setOpenExternalLinks(true);
}

void UILogic::createMdEditor() {
    m_mdEditor = new QTextEdit();
    m_mdEditor->setAcceptRichText(false);
    m_mdEditor->setWordWrapMode(QTextOption::WordWrap);
    m_mdEditor->setLineWrapMode(QTextEdit::WidgetWidth);
    m_mdEditor->setOverwriteMode(false);
    m_mdEditor->setUndoRedoEnabled(true);
}

bool UILogic::addListItem() {
    bool b = false;
    ListItemDialog *dlg = new ListItemDialog();
    journalData data = {};
    if (dlg->Execute(ListItemDialog::create, &data.subject, &data.icon) == QDialog::Accepted) {
        m_id++;
        data.id = m_id;
        data.payload.clear();
        IconList *iconList = new IconList();
        QListWidgetItem *item = new QListWidgetItem(m_listWidget, data.id);
        item->setIcon(iconList->getIconAtIndex(data.icon));
        item->setText(data.subject);
        m_listWidget->addItem(item);
        m_listWidget->setCurrentItem(item);
        m_data.append(data);
        delete iconList;
        m_mdViewer->clear();
        m_mdEditor->clear();
        m_mdEditor->setFocus();
        b = true;
    }
    delete dlg;
    return b;
}

bool UILogic::editListItem() {
    bool changed = false;
    QList<QListWidgetItem*> itemList = m_listWidget->selectedItems();
    if (itemList.empty()) return changed;
    QListWidgetItem *item = itemList[0];
    IconList *iconList = new IconList();
    ListItemDialog *dlg = new ListItemDialog();
    int icon = -1;
    for (auto &p : m_data) {
        if (p.id == item->type()) {
            icon = p.icon;
            break;
        }
    }
    QString text = item->text();
    if (dlg->Execute(ListItemDialog::edit, &text, &icon) == QDialog::Accepted) {
        item->setText(text);
        item->setIcon(iconList->getIconAtIndex(icon));
        for (auto &p : m_data) {
            if (p.id == item->type()) {
                if (icon != p.icon || text != p.subject) changed = true;
                p.icon = icon;
                p.subject = text;
                break;
            }
        }
    }
    delete iconList;
    delete dlg;
    m_listWidget->setCurrentItem(item);
    return changed;
}

bool UILogic::deleteListItem(){
    QList<QListWidgetItem*> itemList = m_listWidget->selectedItems();
    if (itemList.empty()) return false;
    QListWidgetItem *item_d = itemList[0];
    int id = item_d->type();
    m_listWidget->takeItem(m_listWidget->row(item_d));
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data[i].id == id) {
            m_data.remove(i);
            break;
        }
    }
    if (m_listWidget->count() > 0) {
        QListWidgetItem *item_c = m_listWidget->item(0);
        m_listWidget->setCurrentItem(item_c);
    } else {
        m_mdViewer->clear();
        m_mdEditor->clear();
    }
    return true;
}

UILogic::journalData UILogic::getSelectedListItem() {
    QList<QListWidgetItem*> itemList = m_listWidget->selectedItems();
    if (itemList.empty()) return {};
    QListWidgetItem *item_d = itemList[0];
    int id = item_d->type();
    for (auto &p : m_data) {
        if (p.id == id) return p;
    }
    return {};
}

void UILogic::setPayload(int id, QString payload) {
    for (auto &p : m_data) {
        if (p.id == id) {
            p.payload = payload;
            return;
        }
    }
}

QString UILogic::getPayLoad(int id) {
    for (auto &p : m_data) {
        if (p.id == id) return p.payload;
    }
    return "";
}

QJsonObject UILogic::journalDataToJson(journalData data) {
    QJsonObject json = {};
    json["id"] = data.id;
    json["icon"] = data.icon;
    json["subject"] = data.subject;
    json["payload"] = data.payload;
    return json;
}

UILogic::journalData UILogic::journalDataFromJson(QJsonObject json) {
    journalData data = {};
    if (QJsonValue v = json["id"]; v.isDouble()) data.id = v.toInt();
    if (QJsonValue v = json["icon"]; v.isDouble()) data.icon = v.toInt();
    if (QJsonValue v = json["subject"]; v.isString()) data.subject = v.toString();
    if (QJsonValue v = json["payload"]; v.isString()) data.payload = v.toString();
    return data;
}

QByteArray UILogic::dataToJson() {
    QJsonArray array = {};
    QList<QListWidgetItem*> itemList = m_listWidget->selectedItems();
    if (!itemList.empty()) {
        setPayload(itemList[0]->type(), m_mdEditor->toPlainText());
    }
    for (int i = 0; i < m_listWidget->count(); i++) {
        QListWidgetItem* item = m_listWidget->item(i);
        int id = item->type();
        for (auto &p : m_data) {
            if (p.id == id) {
                array.append(journalDataToJson(p));
                break;
            }
        }
    }
    QJsonDocument document(array);
    QByteArray ba = document.toJson();
    return ba;
}

void UILogic::jsonToData(QByteArray ba) {
    QJsonDocument document = QJsonDocument::fromJson(ba);
    if (document.isArray()) {
        QJsonArray array = document.array();
        for (const auto &a : std::as_const(array)) {
            QJsonObject json = a.toObject();
            journalData data = journalDataFromJson(json);
            m_data.append(data);
        }
    }
}

bool UILogic::isAnyItemSelected() {
    return !m_listWidget->selectedItems().empty();
}

int UILogic::getItemCount() {
    return m_listWidget->count();
}

void UILogic::refreshDocument(QString workDir) {
    mc_synchelper->setDocumentPath(workDir);
    mc_synchelper->refreshDocument();
}

void UILogic::openListSettingsDialog() {
    ListFontDialog *dlg = new ListFontDialog();
    QFont font = m_listWidget->font();
    int iconSize = m_listWidget->iconSize().width();
    if (dlg->Execute(&font, &iconSize) == QDialog::Accepted) {
        m_listWidget->setFont(font);
        m_listWidget->setIconSize(QSize(iconSize, iconSize));
    }
    delete dlg;
}

void UILogic::onTextChanged() {
    mc_synchelper->syncToViewer();
    m_mdEditor->setFocus();
}

void UILogic::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    if (previous) {
        setPayload(previous->type(), m_mdEditor->toPlainText());
    }
    if (current) {
        m_textChangeIgnore = true;
        QString md = getPayLoad(current->type());
        m_mdEditor->setPlainText(md);
        m_textChangeIgnore = false;
        m_mdEditor->setFocus();
    }
}

void UILogic::onJBListDropEvent() {
    emit listChanged();
}
