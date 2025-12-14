/////////////////////////////////////////////////////////////////////////////
// Name:        mainwindow.cpp
// Purpose:     The main window
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include "stylesheets.h"
#include "constants.h"
#include "jbpreferences.h"
#include "aboutdialog.h"
#include "io.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mc_uiLogic = new UILogic(this);
    mc_dialogs = new Dialogs(this);
    ui->mainAppInfo->setToolTip(tr("About") + " " + APPNAME);
    ui->mainAppInfo->setText(tr("About") + " " + APPNAME);
    createToolBars();
    createSplitters();
    createStatusBar();
    populateMainMenu();
    setCentralWidget(m_mainSplitter);
    setUnifiedTitleAndToolBarOnMac(true);
    loadPreferences();
    setConnections();
    m_synchronizer =new ScrollSynchronizer(mc_uiLogic->getmdViewer(),
        mc_uiLogic->getmdEditor(), m_editorSplitter);
    m_synchronizer->setEnabled(true);
    fileNew();
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_fontComboBox;
    delete m_fontSizeBox;
    delete m_mainToolBar;
    delete m_mainListToolBar;
    delete m_statusBar;
    delete mc_uiLogic;
    delete m_editorSplitter;
    delete m_treeSplitter;
    delete m_mainSplitter;
    delete mc_dialogs;
}

void MainWindow::createToolBars() {
    m_mainToolBar = new QToolBar;
    m_mainToolBar->setObjectName("mainToolbar");
    m_mainToolBar->setMovable(false);
    m_mainToolBar->setOrientation(Qt::Horizontal);
    m_mainToolBar->setAutoFillBackground(true);
    m_mainToolBar->setIconSize(QSize(DEF_ICONSIZE, DEF_ICONSIZE));
    m_mainToolBar->setFixedHeight(26);
    m_mainListToolBar = new QToolBar;
    m_mainListToolBar->setObjectName("treeToolbar");
    m_mainListToolBar->setMovable(false);
    m_mainListToolBar->setOrientation(Qt::Horizontal);
    m_mainListToolBar->setAutoFillBackground(true);
    m_mainListToolBar->setIconSize(QSize(DEF_ICONSIZE, DEF_ICONSIZE));
    m_mainListToolBar->setFixedHeight(26);
#if defined(Q_OS_WIN)
    m_mainToolBar->setStyleSheet(styleToolBar);
    m_mainListToolBar->setStyleSheet(styleToolBar);
#elif defined(Q_OS_MAC)
    m_mainToolBar->setStyleSheet(styleToolButton);
    m_mainListToolBar->setStyleSheet(styleToolButton);
#endif
    this->addToolBar(Qt::TopToolBarArea, m_mainToolBar);
    this->addToolBar(m_mainListToolBar);
    m_mainListToolBar->addAction(ui->listAddNote);
    m_mainListToolBar->addAction(ui->listEditNote);
    m_mainListToolBar->addAction(ui->listDeleteNote);
    m_mainListToolBar->addSeparator();
    m_mainListToolBar->addAction(ui->listSettings);
    m_mainToolBar->addAction(ui->mainNewFile);
    m_mainToolBar->addAction(ui->mainOpenFile);
    m_mainToolBar->addAction(ui->mainSaveFile);
    m_mainToolBar->addAction(ui->mainSaveFileAs);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(ui->mainEditCopy);
    m_mainToolBar->addAction(ui->mainEditCut);
    m_mainToolBar->addAction(ui->mainEditPaste);
    m_mainToolBar->addAction(ui->mainEditSelectAll);
    m_mainToolBar->addAction(ui->mainEditUndo);
    m_mainToolBar->addAction(ui->mainEditRedo);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(ui->mainEditFormatBold);
    m_mainToolBar->addAction(ui->mainEditFormatItalic);
    m_mainToolBar->addAction(ui->mainEditFormatStrikethrough);
    m_mainToolBar->addAction(ui->mainEditFormatCode);
    m_mainToolBar->addAction(ui->mainEditFormatHighlight);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(ui->mainToggleLock);
    m_mainToolBar->addSeparator();
    QWidget* spacerSmall = new QWidget;
    spacerSmall->setMinimumWidth(25);
    m_mainToolBar->addWidget(spacerSmall);
    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    m_fontComboBox->setWritingSystem(QFontDatabase::Latin);
    m_mainToolBar->addWidget(m_fontComboBox);
    m_fontSizeBox = new QComboBox;
    m_fontSizeBox->addItems(fontSizeList);
    m_mainToolBar->addWidget(m_fontSizeBox);
    QWidget* spacerLarge = new QWidget;
    spacerLarge->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mainToolBar->addWidget(spacerLarge);
    m_mainToolBar->addAction(ui->mainAppInfo);
    m_mainToolBar->addAction(ui->mainAppQuit);
}

void MainWindow::createSplitters() {
    m_mainSplitter = new QSplitter;
    m_mainSplitter->setObjectName("mainSplitter");
    m_mainSplitter->setOrientation(Qt::Horizontal);
    m_treeSplitter = new QSplitter;
    m_treeSplitter->setObjectName("treeSplitter");
    m_treeSplitter->setOrientation(Qt::Vertical);
    m_treeSplitter->setHandleWidth(0);
    m_treeSplitter->addWidget(m_mainListToolBar);
    m_treeSplitter->addWidget(mc_uiLogic->getListWidget());
    m_mainSplitter->addWidget(m_treeSplitter);
    m_editorSplitter = new QSplitter;
    m_editorSplitter->setObjectName("editorSplitter");
    m_editorSplitter->setOrientation(Qt::Vertical);
    m_editorSplitter->addWidget(mc_uiLogic->getmdViewer());
    m_editorSplitter->addWidget(mc_uiLogic->getmdEditor());
    m_mainSplitter->addWidget(m_editorSplitter);
    m_mainSplitter->setChildrenCollapsible(false);
}

void MainWindow::createStatusBar() {
    m_statusBar = new QStatusBar;
    this->setStatusBar(m_statusBar);
#if defined(Q_OS_MAC)
    m_statusBar->setVisible(false);
#endif
}

void MainWindow::populateMainMenu() {
    ui->menuFile->addAction(ui->mainNewFile);
    ui->menuFile->addAction(ui->mainOpenFile);
    ui->menuFile->addAction(ui->mainSaveFile);
    ui->menuFile->addAction(ui->mainSaveFileAs);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->mainAppQuit);
    ui->menuEdit->addAction(ui->mainEditCopy);
    ui->menuEdit->addAction(ui->mainEditCut);
    ui->menuEdit->addAction(ui->mainEditPaste);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditSelectAll);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditUndo);
    ui->menuEdit->addAction(ui->mainEditRedo);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditFormatBold);
    ui->menuEdit->addAction(ui->mainEditFormatItalic);
    ui->menuEdit->addAction(ui->mainEditFormatStrikethrough);
    ui->menuEdit->addAction(ui->mainEditFormatCode);
    ui->menuEdit->addAction(ui->mainEditFormatHighlight);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->listAddNote);
    ui->menuEdit->addAction(ui->listEditNote);
    ui->menuEdit->addAction(ui->listDeleteNote);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainToggleLock);
    ui->menuOptions->addAction(ui->listSettings);
    ui->menuHelp->addAction(ui->mainAppInfo);
}

void MainWindow::setConnections() {
    connect(ui->mainNewFile, &QAction::triggered, this, &MainWindow::onNewFile);
    connect(ui->mainOpenFile, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->mainSaveFile, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->mainSaveFileAs, &QAction::triggered, this, &MainWindow::onSaveFileAs);
    connect(ui->mainEditCopy, &QAction::triggered, this, &MainWindow::onEditCopy);
    connect(ui->mainEditCut, &QAction::triggered, this, &MainWindow::onEditCut);
    connect(ui->mainEditPaste, &QAction::triggered, this, &MainWindow::onEditPaste);
    connect(ui->mainEditSelectAll, &QAction::triggered, this, &MainWindow::onEditSelectAll);
    connect(ui->mainEditUndo, &QAction::triggered, this, &MainWindow::onEditUndo);
    connect(ui->mainEditRedo, &QAction::triggered, this, &MainWindow::onEditRedo);
    connect(ui->mainEditFormatBold, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatItalic, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatStrikethrough, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatCode, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatHighlight, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainToggleLock, &QAction::triggered, this, &MainWindow::onToggleLock);
    connect(ui->mainAppInfo, &QAction::triggered, this, &MainWindow::onAppInfo);
    connect(ui->mainAppQuit, &QAction::triggered, this, &MainWindow::onAppQuit);
    connect(m_fontComboBox, &QFontComboBox::currentFontChanged, this, &MainWindow::onCurrentFontChanged);
    connect(m_fontSizeBox, &QComboBox::currentTextChanged, this, &MainWindow::onCurrentTextChanged);
    connect(ui->listAddNote, &QAction::triggered, this, &MainWindow::onAddNote);
    connect(ui->listEditNote, &QAction::triggered, this, &MainWindow::onEditNote);
    connect(ui->listDeleteNote, &QAction::triggered, this, &MainWindow::onDeleteNote);
    connect(ui->listSettings, &QAction::triggered, this, &MainWindow::onListSettings);
    connect(mc_uiLogic->getmdViewer(), &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(mc_uiLogic, &UILogic::listChanged, this, &MainWindow::onListChanged);
}

void MainWindow::savePreferences() {
    JBPreferences *prefs = new JBPreferences();
    prefs->PushArray(SET_WGEOMETRY, saveGeometry());
    prefs->PushArray(SET_WSTATE, saveState(0));
    prefs->PushArray(SET_SSTATE, m_mainSplitter->saveState());
    prefs->PushString(SET_LASTFOLDER, m_lastFolder);
    prefs->PushFont(SET_EDITORFONT, mc_uiLogic->getmdViewer()->font());
    prefs->PushFont(SET_LISTFONT, mc_uiLogic->getListWidget()->font());
    quint64 iconSize = mc_uiLogic->getListWidget()->iconSize().width();
    prefs->PushNumber(SET_ICONSIZE, iconSize);
    prefs->SavePreferencesToDefaultLocation(SET_COMPANY, APPNAME);
    delete prefs;
}

void MainWindow::loadPreferences() {
    JBPreferences *prefs = new JBPreferences();
    if (prefs->LoadPreferencesFromDefaultLocation(SET_COMPANY, APPNAME)) {
        try {
            restoreGeometry(prefs->PopArray(SET_WGEOMETRY));
            restoreState(prefs->PopArray(SET_WSTATE));
            m_mainSplitter->restoreState(prefs->PopArray(SET_SSTATE));
            m_lastFolder = prefs->PopString(SET_LASTFOLDER);
            QFont efont = prefs->PopFont(SET_EDITORFONT);
            if (!efont.family().isEmpty()) {
                m_fontComboBox->setCurrentFont(efont);
                QString esize = QString::number(efont.pointSize());
                if (fontSizeList.indexOf(esize) >= 0) m_fontSizeBox->setCurrentText(esize);
                mc_uiLogic->getmdViewer()->setFont(efont);
                mc_uiLogic->getmdEditor()->setFont(efont);
            }
            QFont lFont = prefs->PopFont(SET_LISTFONT);
            if (!lFont.family().isEmpty()) {
                mc_uiLogic->getListWidget()->setFont(lFont);
            }
            quint64 iconSize = prefs->PopNumber(SET_ICONSIZE);
            if (iconSize < MIN_ICONSIZE || iconSize > MAX_ICONSIZE) iconSize = DEF_ICONSIZE;
            mc_uiLogic->getListWidget()->setIconSize(QSize(iconSize, iconSize));
        }
        catch (...) {}
    }
    delete prefs;
    if (m_lastFolder.isEmpty()) m_lastFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

void MainWindow::setLockStatus() {
    bool b = ui->mainToggleLock->isChecked();
    bool c = mc_uiLogic->getItemCount() > 0;
    if (b) {
        ui->mainToggleLock->setIcon(QIcon(":/tb/assets/mainTB_Locked.svg"));
        ui->mainToggleLock->setText(tr("Edit mode"));
        ui->mainToggleLock->setToolTip(tr("Switch to edit mode"));
    }
    else {
        ui->mainToggleLock->setIcon(QIcon(":/tb/assets/mainTB_Unlocked.svg"));
        ui->mainToggleLock->setText(tr("View mode"));
        ui->mainToggleLock->setToolTip(tr("Switch to view mode"));
    }
    ui->listAddNote->setEnabled(!b);
    ui->listDeleteNote->setEnabled(!b && c);
    ui->listEditNote->setEnabled(!b && c);
    ui->mainEditCopy->setEnabled(!b && c);
    ui->mainEditCut->setEnabled(!b && c);
    ui->mainEditPaste->setEnabled(!b && c);
    ui->mainEditUndo->setEnabled(!b && c);
    ui->mainEditRedo->setEnabled(!b && c);
    enableFormatActions(!b && c);
    mc_uiLogic->getmdEditor()->setReadOnly(b || !c);
    mc_uiLogic->getListWidget()->setDragEnabled(!b && c);
    if (b) m_editorSplitter->setSizes({1, 0});
    else m_editorSplitter->setSizes({1, 1});
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if (answer == QMessageBox::Cancel) {
            e->ignore();
            return;
        }
        if (answer == QMessageBox::Yes) {
            if (!fileSave()) {
                e->ignore();
                return;
            }
        }
    }
    savePreferences();
    setWindowModified(false);
    e->accept();
}

void MainWindow::onTextChanged() {
    if (!mc_uiLogic->getTextChangeIgnore()) setWindowModified(true);
}

// font combobox
void MainWindow::onCurrentFontChanged(const QFont font) {
    QFont f = font;
    if (!f.family().isEmpty()) {
        int s = m_fontSizeBox->currentText().toInt();
        f.setPointSize(s);
        mc_uiLogic->getmdViewer()->setFont(f);
        mc_uiLogic->getmdEditor()->setFont(f);
    }
}

// font size combobox
void MainWindow::onCurrentTextChanged(const QString size) {
    int s = size.toInt();
    QFont font = mc_uiLogic->getmdViewer()->font();
    font.setPointSize(s);
    mc_uiLogic->getmdViewer()->setFont(font);
    mc_uiLogic->getmdEditor()->setFont(font);
}

void MainWindow::onEditFormat() {
    QTextCursor cursor = mc_uiLogic->getmdEditor()->textCursor();
    if (cursor.hasSelection()) {
        int s_begin = cursor.selectionStart();
        int s_end = cursor.selectionEnd();
        QAction* action = qobject_cast<QAction*>(sender());
        if (!action) return;
        QString format{};
        if (action == ui->mainEditFormatBold) {
            format = "**";
        } else if (action == ui->mainEditFormatItalic) {
            format = "*";
        } else if (action == ui->mainEditFormatStrikethrough) {
            format = "~";
        } else if (action == ui->mainEditFormatCode) {
            format = "`";
        } else if (action == ui->mainEditFormatHighlight) {
            format = "==";
        }
        if (!format.isEmpty()) {
            cursor.setPosition(s_end, QTextCursor::MoveAnchor);
            cursor.insertText(format);
            cursor.setPosition(s_begin, QTextCursor::MoveAnchor);
            cursor.insertText(format);
        }
    }
}

void MainWindow::onNewFile() {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if(answer == QMessageBox::Cancel) return;
        if(answer == QMessageBox::Yes) {
            if (!fileSave()) return;
        }
    }
    fileNew();
}

void MainWindow::onListChanged() {
    setWindowModified(true);
}

void MainWindow::onOpenFile() {
    fileOpen();
}

void MainWindow::onSaveFile() {
    fileSave();
}

void MainWindow::onSaveFileAs() {
    fileSave(true);
}

void MainWindow::onEditCopy() {
    mc_uiLogic->getmdEditor()->copy();
}

void MainWindow::onEditCut() {
    if (!mc_uiLogic->getmdEditor()->isReadOnly()) mc_uiLogic->getmdEditor()->cut();
}

void MainWindow::onEditPaste() {
    if (mc_uiLogic->getmdEditor()->canPaste() && !mc_uiLogic->getmdEditor()->isReadOnly()) {
        mc_uiLogic->getmdEditor()->paste();
    }
}

void MainWindow::onEditSelectAll() {
    mc_uiLogic->getmdEditor()->selectAll();
}

void MainWindow::onEditUndo() {
    if (!mc_uiLogic->getmdEditor()->isReadOnly()) mc_uiLogic->getmdEditor()->undo();
}

void MainWindow::onEditRedo() {
    if (!mc_uiLogic->getmdEditor()->isReadOnly()) mc_uiLogic->getmdEditor()->redo();
}

void MainWindow::onToggleLock() {
    setLockStatus();
}

void MainWindow::onAppInfo() {
    AboutDialog dlg;
    dlg.setParent(this);
    int pw = width();
    int ph = height() + m_mainToolBar->height();
    int mw = dlg.width();
    int mh = dlg.height();
    if (pw >= mw && ph >= mh) {
        dlg.move((pw - mw) / 2, (ph - mh) / 2);
    }
    dlg.exec();
}

void MainWindow::onAppQuit() {
    close();
}

void MainWindow::onAddNote() {
    if (mc_uiLogic->addListItem()) settingsAfterAddNote();
}

void MainWindow::onEditNote() {
    if (mc_uiLogic->editListItem()) settingsAfterEditNote();
}

void MainWindow::onDeleteNote() {
    if (mc_uiLogic->isAnyItemSelected()) {
        if (mc_dialogs->showDialogDeleteNote() == QMessageBox::Yes) {
            if (mc_uiLogic->deleteListItem()) {
                settingsAfterDeleteNote();
            }
        }
    }
}

void MainWindow::onListSettings() {
    mc_uiLogic->openListSettingsDialog();
}

void MainWindow::fileNew() {
    m_fileName.clear();
    mc_uiLogic->resetAll();
    settingsAfterNew();
}

bool MainWindow::fileOpen() {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if (answer == QMessageBox::Cancel) return true;
        if (answer == QMessageBox::Ok) {
            return fileSave();
        }
    }
    QString fName = mc_dialogs->showOpenFileDialog(m_lastFolder);
    if (!fName.isEmpty()) {
        QFileInfo fileInfo(fName);
        m_lastFolder = fileInfo.path();
        m_fileName = fileInfo.fileName();
        QByteArray ba;
        Io* io = new Io();
        QString result = io->load(fName, &ba);
        delete io;
        if (result.isEmpty()) {
            mc_uiLogic->startUp(ba, m_lastFolder);
            QString title = QString(APPNAME) + " - " + fileInfo.fileName() + PLACEHOLDER;
            setWindowTitle(title);
            settingsAfterLoad();
            return true;
        } else mc_dialogs->showDialogDisplayError(result);
    }
    return false;
}

bool MainWindow::fileSave(bool saveAs) { //saveAs default false
    QString fName;
    if (m_lastFolder.isEmpty() || m_fileName.isEmpty() || saveAs) {
        fName = mc_dialogs->showSaveFileDialog(m_lastFolder, m_fileName);
        if (fName.isEmpty()) return false;
        QFileInfo fileInfo(fName);
        m_lastFolder = fileInfo.path();
        m_fileName = fileInfo.fileName();
    } else {
        QFileInfo fileInfo(m_lastFolder, m_fileName);
        fName = fileInfo.absoluteFilePath();
    }
    QByteArray ba = mc_uiLogic->dataToJson();
    Io* io = new Io();
    QString result = io->save(fName, ba);
    delete io;
    if (result.isEmpty()) {
        mc_uiLogic->refreshDocument(m_lastFolder);
        QString title = QString(APPNAME) + " - " + m_fileName + PLACEHOLDER;
        setWindowTitle(title);
        setWindowModified(false);
        return true;

    } else mc_dialogs->showDialogDisplayError(result);
    return false;
}

void MainWindow::enableFormatActions(bool enabled) {
    ui->mainEditFormatBold->setEnabled(enabled);
    ui->mainEditFormatItalic->setEnabled(enabled);
    ui->mainEditFormatStrikethrough->setEnabled(enabled);
    ui->mainEditFormatCode->setEnabled(enabled);
    ui->mainEditFormatHighlight->setEnabled(enabled);
}

void MainWindow::settingsAfterLoad() {
    //enable save, set locked, enable lock, set readonly
    ui->mainSaveFile->setEnabled(true);
    ui->mainSaveFileAs->setEnabled(true);
    ui->mainToggleLock->setChecked(true);
    setLockStatus();
    mc_uiLogic->getmdEditor()->setReadOnly(true);
    setWindowModified(false);
}

void MainWindow::settingsAfterNew() {
    //disable save, set unlocked, disable lock, set readonly
    ui->mainSaveFile->setEnabled(false);
    ui->mainSaveFileAs->setEnabled(false);
    ui->mainToggleLock->setChecked(false);
    enableFormatActions(false);
    setLockStatus();
    ui->listEditNote->setEnabled(false);
    ui->listDeleteNote->setEnabled(false);
    //user must add new note first!
    mc_uiLogic->getmdEditor()->setReadOnly(true);
    QString title = QString(APPNAME) + " - " + mc_dialogs->UNTITLED_DOCUMENT + PLACEHOLDER;
    setWindowTitle(title);
    setWindowModified(false);
}

void MainWindow::settingsAfterAddNote() {
    //enable save, set unlocked, enable lock, set readwrite
    ui->mainSaveFile->setEnabled(true);
    ui->mainSaveFileAs->setEnabled(true);
    ui->mainToggleLock->setChecked(false);
    enableFormatActions(true);
    setLockStatus();
    ui->listEditNote->setEnabled(true);
    ui->listDeleteNote->setEnabled(true);
    mc_uiLogic->getmdEditor()->setReadOnly(false);
    setWindowModified(true);
}

void MainWindow::settingsAfterEditNote() {
    setWindowModified(true);
}

void MainWindow::settingsAfterDeleteNote() {
    bool b = mc_uiLogic->getItemCount() > 0;
    if (!b) mc_uiLogic->getmdViewer()->clear(); //no note left, clear text
    setWindowModified(b);
    ui->mainSaveFile->setEnabled(b);
    ui->mainSaveFileAs->setEnabled(b);
    ui->mainToggleLock->setEnabled(b);
    ui->listEditNote->setEnabled(b);
    ui->listDeleteNote->setEnabled(b);
    enableFormatActions(b);
    bool c = b || ui->mainToggleLock->isChecked();
    mc_uiLogic->getmdEditor()->setReadOnly(!c);
}
