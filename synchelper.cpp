/////////////////////////////////////////////////////////////////////////////
// Name:        synchelper.cpp
// Purpose:     Keep MD viewer in sync with MD editor
// Author:      Jan Buchholz
// Created:     2025-11-26
// Changed:     2026-04-07
/////////////////////////////////////////////////////////////////////////////

#include "synchelper.h"
#include <QTextBlock>
#include <QScrollBar>
#include <QRegularExpression>
#include "md4c/src/md4c-html.h"

SyncHelper::SyncHelper(QTextEdit* editor, QTextBrowser* viewer)
    : m_editor(editor), m_viewer(viewer) {
    mc_handler = new ImageHandler;
    m_documentPath.clear();
}

SyncHelper::~SyncHelper() {
    delete mc_handler;
}

void SyncHelper::syncToViewer() {
    if (!m_editor || !m_viewer) return;
    // insert marker in editor
    m_editor->blockSignals(true);
    QTextCursor eCursor = m_editor->textCursor();
    int oldPos = eCursor.position();
    QTextBlock eBlock = eCursor.block();
    if (eBlock.isValid()) {
        eCursor.movePosition(QTextCursor::EndOfBlock);
        eCursor.insertText(c_anchor);
        // old position + marker length
        eCursor.setPosition(oldPos + c_anchor.length());
        m_editor->setTextCursor(eCursor);
    }
    m_editor->blockSignals(false);
    QString markdown = m_editor->toPlainText();
    // emulate highlighting
    // QRegularExpression re("==([^\\s].*?[^\\s])==");
    QRegularExpression re("==([^=]+)=="); // more tolerant
    markdown.replace(re, c_highlighting);
    QByteArray ba = markdown.toUtf8();
    std::string html;
    // convert to html using md4c-html
    md_html(ba.constData(), ba.size(),
        [](const MD_CHAR* text, MD_SIZE size, void* userdata) {
            auto* out = static_cast<std::string*>(userdata);
            out->append(text, size);
        },
        &html, MD_FLAG_TABLES | MD_FLAG_STRIKETHROUGH | MD_FLAG_HARD_SOFT_BREAKS, 0);
    QString content = QString::fromUtf8( html.data(), html.size());
    m_viewer->setHtml(content);
    // remove marker in editor
    m_editor->blockSignals(true);
    QTextCursor eFound = m_editor->document()->find(c_anchor);
    if (!eFound.isNull()) {
        eFound.removeSelectedText(); // delete marker
    }
    eCursor.setPosition(oldPos);
    m_editor->setTextCursor(eCursor);
    m_editor->blockSignals(false);
    // load and scale pictures
    processImages();
    // scroll to position by searching for anchor
    QTextDocument* doc = m_viewer->document();
    doc->markContentsDirty(0, doc->characterCount()); // force html reformatting
     for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
        for (auto it = block.begin(); !it.atEnd(); it++) {
            QTextFragment frag = it.fragment();
            if (frag.isValid()) {
                if (frag.charFormat().isAnchor()) {
                    QTextCharFormat ch = frag.charFormat().toCharFormat();
                    const QStringList list = ch.anchorNames();
                    for (const auto &name : list) {
                        if (name == c_anchor_name) {
                            QTextCursor cursor(block);
                            cursor.movePosition(QTextCursor::EndOfBlock);
                            m_viewer->setTextCursor(cursor);
                            m_viewer->update();
                            m_viewer->ensureCursorVisible();                           
                            return;
                        }
                    }
                }
            }
        }
    }
}

void SyncHelper::processImages() {
    m_viewer->blockSignals(true);
    QScrollBar *vbar = m_viewer->verticalScrollBar();
    int maxWidth = m_viewer->viewport()->width() - vbar->sizeHint().width() * 2;
    QTextDocument *doc = m_viewer->document();
    // fetch images
    QTextBlock block = doc->begin();
    while (block.isValid()) {
        for (auto it = block.begin(); !it.atEnd(); it++) {
            QTextFragment frag = it.fragment();
            if (frag.isValid() && frag.charFormat().isImageFormat()) {
                QTextImageFormat imgFmt = frag.charFormat().toImageFormat();
                QImage image = mc_handler->getFromCache(imgFmt.name(), m_documentPath);
                if (!image.isNull()) {
                    QSize size = image.size();
                    doc->addResource(QTextDocument::ImageResource, QUrl(imgFmt.name()), image);
                    if (size.width() > maxWidth) {
                        int newHeight = size.height() * maxWidth / size.width();
                        imgFmt.setWidth(maxWidth);
                        imgFmt.setHeight(newHeight);
                    } else {
                        imgFmt.setWidth(size.width());
                        imgFmt.setHeight(size.height());
                    }
                    QTextCursor cursor(doc);
                    cursor.setPosition(frag.position());
                    cursor.setPosition(frag.position() + frag.length(), QTextCursor::KeepAnchor);
                    cursor.setCharFormat(imgFmt);
                }
            }
        }
        block = block.next();
    }
    m_viewer->blockSignals(false);
}

void SyncHelper::refreshDocument() {
    mc_handler->invalidateCache();
    syncToViewer();
}
