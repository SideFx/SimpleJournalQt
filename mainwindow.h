/////////////////////////////////////////////////////////////////////////////
// Name:        mainwindow.h
// Purpose:     The main window (header)
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QMainWindow>
#include <QToolBar>
#include <QSplitter>
#include <QCloseEvent>
#include <QFontComboBox>
#include <QCheckBox>
#include <QLabel>
#include "dialogs.h"
#include "uilogic.h"
#include "scrollsynchronizer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QToolBar *m_mainToolBar, *m_mainListToolBar;
    QStatusBar *m_statusBar;
    QSplitter *m_mainSplitter, *m_treeSplitter, *m_editorSplitter;
    QFontComboBox *m_fontComboBox;
    QComboBox *m_fontSizeBox;
    ScrollSynchronizer *m_synchronizer;
    UILogic *mc_uiLogic;
    QString m_lastFolder;
    QString m_fileName;
    Dialogs *mc_dialogs;
    const QStringList fontSizeList = {"8", "9", "10", "11", "12", "13", "14", "16", "18"};
    void createToolBars();
    void createSplitters();
    void createStatusBar();
    void populateMainMenu();
    void setConnections();
    void savePreferences();
    void loadPreferences();
    void setLockStatus();
    void fileNew();
    bool fileOpen();
    bool fileSave(bool saveAs = false);
    void enableFormatActions(bool enabled);
    void settingsAfterLoad();
    void settingsAfterNew();
    void settingsAfterAddNote();
    void settingsAfterEditNote();
    void settingsAfterDeleteNote();

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveFileAs();
    void onListSettings();
    void onAppInfo();
    void onAppQuit();
    void onEditCopy();
    void onEditCut();
    void onEditPaste();
    void onEditSelectAll();
    void onEditUndo();
    void onEditRedo();
    void onToggleLock();
    void onAddNote();
    void onDeleteNote();
    void onEditNote();

    void onCurrentFontChanged(const QFont);
    void onCurrentTextChanged(const QString);
    void onTextChanged();
    void onListChanged();
    void onEditFormat();

};

