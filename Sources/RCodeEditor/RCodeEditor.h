//==-- RCodeEditor.h - RCodeEditor Main Interface Header -----------------*- C++ -*-==//
//* Based on QCodeEditor Project by Megaxela                                         *//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.                      *//
//* Retoccilus licensed under GNU GPL v3 license.                                    *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.           *//
//* QCodeEditor licensed under MIT License                                           *//
//* See <https://github.com/Megaxela/QCodeEditor>. for license information.          *//
//==-----------------------------------------------------------------------------====*//

#ifndef RCodeEditor_Main_h
#define RCodeEditor_Main_h

#pragma once

#define Reto_SideBar_Default_Width 50

#include <QPlainTextEdit>
#include "RBorderTextProperty.h"

class RSideBar;
class QCompleter;
class RSyntaxStyle;
class RStyleSyntaxHighlighter;
class RBorderTextProperty;

/**
 * @class RCodeEditor
 * @brief This class prevent you a code editor
 */
class RCodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param widget (Parent widget, means this editor component need to be insert to another parent widget)
     */
    explicit RCodeEditor(QWidget *widget = nullptr);
    [[maybe_unused]] int getFirstVisibleBlock();
    [[maybe_unused]] void setHighlighter(RStyleSyntaxHighlighter *highlighter);
    void setSyntaxStyle(RSyntaxStyle *style);
    [[maybe_unused]] void setAutoParentheses(bool enabled);
    [[maybe_unused]] bool autoParentheses() const;
    [[maybe_unused]] void setTabReplace(bool enabled);
    [[maybe_unused]] bool tabReplace() const;
    [[maybe_unused]] void setTabReplaceSize(int val);
    [[maybe_unused]] int tabReplaceSize() const;
    [[maybe_unused]] void setAutoIndentation(bool enabled);
    [[maybe_unused]] bool autoIndentation() const;
    [[maybe_unused]] void setCompleter(QCompleter *completer);
    [[nodiscard]] QCompleter *completer() const;
    void refreshLayout();
    void sidebarPaintEvent(QPaintEvent *pEvent);
    int sidebarWidth();
signals:
    void commentLine_NoSelection();
    void unCommentLine_NoSelection();
    void commentBlock_NoSelection();
    void unCommentBlock_NoSelection();
public slots:
    void insertCompletion(QString s);
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &rect);
    void updateExtraSelection();
    void updateStyle();
    void onSelectionChanged();

    void commentLine();
    void uncommentLine();
    void commentBlock();
    void uncommentBlock();

    //Folding
    void fold();
    void unfold();
protected:
    void insertFromMimeData(const QMimeData *source) override;
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
private:
    void initDocumentLayoutHandlers();
    void initFont();
    void handleSelectionQuery(const QTextCursor& cursor);
    void updateLineGeometry();
    bool proceedCompleterBegin(QKeyEvent *e);
    void proceedCompleterEnd(QKeyEvent *e);
    [[nodiscard]] QChar charUnderCursor(int offset = 0) const;
    [[nodiscard]] QString wordUnderCursor() const;
    void highlightCurrentLine(QList<QTextEdit::ExtraSelection> &extraSelection);
    void highlightParenthesis(QList<QTextEdit::ExtraSelection> &extraSelection);
    int getIndentationSpaces();

private:
    RStyleSyntaxHighlighter *highlighter;
    RSyntaxStyle *syntaxStyle;
    RSideBar *sidebar;
    RBorderTextProperty *btp;
    QCompleter *pCompleter;

    bool pAutoIndentation;
    bool pAutoParenthese;
    bool pReplaceTab;
    QString pTabReplace;

    void deIndent(int indentationLevel);

    void updateCodeEditorWidth();
};

#endif