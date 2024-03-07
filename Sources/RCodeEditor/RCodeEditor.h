#ifndef RCodeEditor_Main_h
#define RCodeEditor_Main_h

#pragma once

#include <QPlainTextEdit>

class RSideBar;
class QCompleter;
class RSyntaxStyle;
class RStyleSyntaxHighlighter;
class RBorderTextProperty;

class RCodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit RCodeEditor(QWidget *widget = nullptr);

    RCodeEditor(const RCodeEditor &) = delete;
    RCodeEditor &operator=(const RCodeEditor &) = delete;

    int getFirstVisibleBlock();
    void setHighlighter(RStyleSyntaxHighlighter *highlighter);
    void setSyntaxStyle(RSyntaxStyle *style);
    void setAutoParentheses(bool enabled);
    bool autoParentheses() const;
    void setTabReplace(bool enabled);
    bool tabReplace() const;
    void setTabReplaceSize(int val);
    int tabReplaceSize() const;
    void setAutoIndentation(bool enabled);
    bool autoIndentation() const;
    void setCompleter(QCompleter *completer);
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

    void toggleFold(QTextBlock block);
    bool isFoldable(const QTextBlock &block);
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
};

#endif