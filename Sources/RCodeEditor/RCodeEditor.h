#ifndef RCodeEditor_Main_h
#define RCodeEditor_Main_h

#pragma once

#define Reto_SideBar_Default_Width 50

#include <QPlainTextEdit>
#include "foldingHandler.h"
#include "RBorderTextProperty.h"

class RSideBar;
class QCompleter;
class RSyntaxStyle;
class RStyleSyntaxHighlighter;
class RBorderTextProperty;

class RCodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
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
    void sidebarPaintEvent(QPaintEvent *pEvent);
    int sidebarWidth();
public slots:
    void insertCompletion(QString s);
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &rect);
    void updateExtraSelection();
    void updateStyle();
    void onSelectionChanged();

    //Folding
    void fold();
    void unfold();
    void framed();
    void unframed();
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
    QChar charUnderCursor(int offset = 0) const;
    QString wordUnderCursor() const;
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
};

#endif