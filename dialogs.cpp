/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.cpp
// Purpose:     File- and message dialogs
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-04-05
/////////////////////////////////////////////////////////////////////////////

#include "dialogs.h"
#include <QFileDialog>
#include "constants.h"

Dialogs::Dialogs(QWidget *parent) : QObject{parent} {
    mainWindow = qobject_cast<QMainWindow*>(parent);
}

Dialogs::~Dialogs() {
}

QString Dialogs::showOpenFileDialog(QString folder) {
    QFileDialog openDialog;
    QString fileName = "";
    openDialog.setParent(mainWindow, Qt::Dialog|
                                     Qt::WindowSystemMenuHint|
                                     Qt::WindowCloseButtonHint|
                                     Qt::WindowTitleHint|
                                     Qt::CustomizeWindowHint);
    openDialog.setDirectory(folder);
    openDialog.setAcceptMode(QFileDialog::AcceptOpen);
    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setNameFilter(FILE_FILTER);
    openDialog.setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
    openDialog.setViewMode(QFileDialog::Detail);
    if (openDialog.exec() == QDialog::Accepted) {
        QStringList files = openDialog.selectedFiles();
        if (!files.empty()) fileName = files.first();
    }
    return fileName;
}

QString Dialogs::showSaveFileDialog(QString folder, QString fileName) {
    QFileDialog saveDialog;
    QString fname;
    if (fileName.isEmpty()) fname = UNTITLED_DOCUMENT; else fname = fileName;
    saveDialog.setParent(mainWindow, Qt::Dialog|
                                     Qt::WindowSystemMenuHint|
                                     Qt::WindowCloseButtonHint|
                                     Qt::WindowTitleHint|
                                     Qt::CustomizeWindowHint);
    saveDialog.setDirectory(folder);
    saveDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveDialog.setFileMode(QFileDialog::AnyFile);
    saveDialog.setNameFilter(FILE_FILTER);
    saveDialog.setDefaultSuffix(DEFAULT_SUFFIX);
    saveDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    saveDialog.setViewMode(QFileDialog::Detail);
    saveDialog.selectFile(fileName);
    if (saveDialog.exec() == QDialog::Accepted) {
        QStringList files = saveDialog.selectedFiles();
        if (!files.empty()) fileName = files.first();
    }
    return fileName;
}

int Dialogs::showDialogUnsavedChanges() {
    return yesNoCancelDialog(APPNAME, MSG_WANTSAVE, QMessageBox::Question);
}

int Dialogs::showDialogDeleteNote() {
    return yesNoCancelDialog(APPNAME, MSG_DELETENOTE, QMessageBox::Question);
}

void Dialogs::showDialogDisplayError(QString message) {
    QMessageBox msg;
    msg.setParent(mainWindow, Qt::Dialog|
                              Qt::WindowSystemMenuHint|
                              Qt::WindowCloseButtonHint|
                              Qt::WindowTitleHint|
                              Qt::CustomizeWindowHint|
                              Qt::MSWindowsFixedSizeDialogHint);
    msg.setWindowModality(Qt::WindowModal);
    msg.setText(message);
    msg.setWindowTitle(APPNAME);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Critical);
    msg.exec();
}

int Dialogs::yesNoCancelDialog(QString title, QString message, QMessageBox::Icon icon) {
    QMessageBox msg;
    msg.setParent(mainWindow, Qt::Dialog|
                              Qt::WindowSystemMenuHint|
                              Qt::WindowCloseButtonHint|
                              Qt::WindowTitleHint|
                              Qt::CustomizeWindowHint|
                              Qt::MSWindowsFixedSizeDialogHint);
    msg.setWindowModality(Qt::WindowModal);
    msg.setText(message);
    msg.setWindowTitle(title);
    msg.setIcon(icon);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Cancel);
    return msg.exec();
}

