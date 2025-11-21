/////////////////////////////////////////////////////////////////////////////
// Name:        uilogic.h
// Purpose:     QListWidget & QTextEdit logic, JSON conversion (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QTextEdit>
#include <QTextBrowser>
#include <QJsonObject>
#include <QByteArray>
#include "jlistwidget.h"

class UILogic : public QObject {
    Q_OBJECT

public:
    explicit UILogic(QObject *parent = nullptr);
    ~UILogic();
    typedef struct {
        int id;
        int icon;
        QString subject;
        QString payload;
    } journalData;
    QListWidget* getListWidget() const {return m_listWidget;}
    QTextEdit* getmdViewer() const {return m_mdViewer;}
    QTextEdit* getmdEditor() const {return m_mdEditor;}
    void resetAll();
    void startUp(const QByteArray ba);
    bool addListItem();
    bool editListItem();
    bool deleteListItem();
    UILogic::journalData getSelectedListItem();
    bool getTextChangeIgnore() const {return m_textChangeIgnore;}
    void openListSettingsDialog();
    bool isAnyItemSelected();
    int getItemCount();
    QByteArray dataToJson();

private:
    JListWidget *m_listWidget;
    QTextBrowser *m_mdViewer;
    QTextEdit *m_mdEditor;
    QVector<journalData> m_data;
    int m_id;
    bool m_textChangeIgnore;
    void createListWidget();
    void createMdViewer();
    void createMdEditor();
    void setPayload(int id, QString payload);
    QString getPayLoad(int id);
    void jsonToData(QByteArray ba);
    QJsonObject journalDataToJson(journalData data);
    journalData journalDataFromJson(QJsonObject json);

private slots:
    void onCurrentItemChanged(QListWidgetItem*, QListWidgetItem*);
    void onJListDropEvent();
    void onTextChanged();

signals:
    void listChanged();

};

