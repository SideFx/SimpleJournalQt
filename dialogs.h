/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.h
// Purpose:     File- and message dialogs header
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-05-22
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>

class Dialogs : public QObject {
    Q_OBJECT

public:
    explicit Dialogs(QMainWindow *parent = nullptr);
    ~Dialogs();
    QString showOpenFileDialog(QString folder);
    QString showSaveFileDialog(QString folder, QString fileName);
    int showDialogUnsavedChanges();
    int showDialogDeleteNote();
    void showDialogDisplayError(QString message);

    const QString UNTITLED_DOCUMENT = tr("NoName.smp");

private:
    QMainWindow* m_mainWindow;
    int yesNoCancelDialog(QString title, QString message, QMessageBox::Icon icon);
    QString const MSG_WANTSAVE = QObject::tr("Should the changes be saved before closing?\n"
                                             "If you don't save, the changes will be lost.");
    QString const MSG_DELETENOTE = QObject::tr("Do you really want to delete the selected note?");
    QString const FILE_FILTER = QObject::tr("SimpleJournal files (*.smp)");
    QString const DEFAULT_SUFFIX = ".smp";

};

