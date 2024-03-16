/*
 * Note: This project is inspired by QCodeEditor project.
 * The original project can be found at <https://github.com/Megaxela/QCodeEditor>.
 * This project is a modified version of the original project.
 * The original project is licensed under the MIT License.
 *
 * Original Creator: Megaxela
 * Modified by: Ryan Almond
 */
#include "RSidebar.h"
#include "RCodeEditor.h"
#include "RBorderTextProperty.h"
#include "RStyleSyntaxHighlighter.h"
#include "RSyntaxStyle.h"

#include <QPlainTextEdit>
#include <QTextBlock>
#include <QPaintEvent>
#include <QFontDatabase>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QCursor>
#include <QCompleter>
#include <QAbstractItemView>
#include <QShortcut>
#include <QMimeData>

static QVector<QPair<QString, QString>> parentheses = {
        {"(",  ")"},
        {"{",  "}"},
        {"[",  "]"},
        {"\"", "\""}, //Quotation mark need \ to mark Escape Character
        {"'",  "'"},
};

RCodeEditor::RCodeEditor(QWidget *widget) :
        QPlainTextEdit(widget),
        highlighter(nullptr),
        syntaxStyle(nullptr),
        sidebar(new RSideBar(this)),
        pCompleter(nullptr),
        btp(new RBorderTextProperty(this)),
        pAutoIndentation(true),
        pAutoParenthese(true),
        pReplaceTab(true),
        pTabReplace(QString(4, ' '))
{
    initDocumentLayoutHandlers();
    initFont();

    QShortcut *foldShortcut = new QShortcut(QKeySequence("F5"), this);
    QShortcut *unfoldShortcut = new QShortcut(QKeySequence("F6"), this);

    connect(foldShortcut, &QShortcut::activated, this, &RCodeEditor::fold);
    connect(unfoldShortcut, &QShortcut::activated, this, &RCodeEditor::unfold);

    connect(document(),&QTextDocument::blockCountChanged,this,&RCodeEditor::updateLineNumberAreaWidth);
    connect(verticalScrollBar(),&QScrollBar::valueChanged,[this](int) { sidebar->update(); });
    connect(this,&QPlainTextEdit::cursorPositionChanged,this,&RCodeEditor::updateExtraSelection);
    connect(this,&QPlainTextEdit::selectionChanged,this,&RCodeEditor::onSelectionChanged);

    setSyntaxStyle(RSyntaxStyle::defaultStyle());
}

void RCodeEditor::initDocumentLayoutHandlers() {
//    document()->documentLayout()->registerHandler(RBorderTextProperty::type(), btp);
}

/// \brief Wheel event, handle the Control + Mouse Scroll to zoom in or out.
/// \param e
void RCodeEditor::wheelEvent(QWheelEvent *e) {
    if (e->modifiers() & Qt::ControlModifier) {
        qreal fontsize = font().pointSizeF();
        qreal sbfontsize = sidebar->font().pointSizeF();
        int delta = e->angleDelta().y();

        if (delta > 0) {
            fontsize += 1.0;
            sbfontsize += 1.0;
        }
        else {
            fontsize -= 1.0;
            sbfontsize -= 1.0;
        }

        QFont tfont = font();
        tfont.setPointSizeF(fontsize);
        this->setFont(tfont);

        QFont sbfont = sidebar->font();
        sbfont.setPointSizeF(sbfontsize);
        sidebar->setFont(sbfont);

        e->accept();
    }
    else {
        QPlainTextEdit::wheelEvent(e);
    }
}

/// \brief Initialize the font for the code editor
void RCodeEditor::initFont() {
    QFont fnt = QFont("Consolas");
    fnt.setFixedPitch(true);
    fnt.setPointSize(10);

    setFont(fnt);
}

/// \brief Set the highlighter for the code editor.
/// \todo This function may used in future, but not now.
void RCodeEditor::setHighlighter(RStyleSyntaxHighlighter *xhighlighter) {
    if (highlighter) {
        highlighter->setDocument(nullptr);
    }

    highlighter = xhighlighter;

    if (highlighter) {
        highlighter->setSyntaxStyle(syntaxStyle);
        highlighter->setDocument(document());
    }
}

/// \brief Set the syntax style for the code editor.
/// \todo This function may used in future, but not now.
void RCodeEditor::setSyntaxStyle(RSyntaxStyle *style) {
    syntaxStyle = style;

//    btp->setSyntaxStyle(syntaxStyle);

    if (highlighter) {
        highlighter->setSyntaxStyle(syntaxStyle);
    }

    updateStyle();
}

/// \brief Update the highlight style for the code editor.
/// \todo This function may used in future, but not now.
void RCodeEditor::updateStyle() {
    if (highlighter) {
        highlighter->rehighlight();
    }

    if (syntaxStyle) {
        QPalette currentPalette = palette();
        currentPalette.setColor(QPalette::ColorRole::Text,syntaxStyle->getFormat("Text").foreground().color());
        currentPalette.setColor(QPalette::Base,syntaxStyle->getFormat("Text").background().color());
        currentPalette.setColor(QPalette::Highlight,syntaxStyle->getFormat("Selection").background().color());
        setPalette(currentPalette);
    }

    updateExtraSelection();
}

/// \brief Handle the collapse (folding) selection changing of the code editor
/// \todo This function may used in future, but not now.
/// \bug This function may has some unexpected bug need to fix.
void RCodeEditor::onSelectionChanged() {
    QString selected = textCursor().selectedText();
    QTextCursor cursor = textCursor();

    if (cursor.isNull()) {
        return;
    }

    cursor.movePosition(QTextCursor::MoveOperation::Left);
    cursor.select(QTextCursor::SelectionType::WordUnderCursor);

    QSignalBlocker blocker(this);
//    RBorderTextProperty::unframe(cursor);
    if (selected.size() > 1 &&
        cursor.selectedText() == selected) {
        QTextCursor backup = textCursor();
        handleSelectionQuery(cursor);
        setTextCursor(backup);
    }
}

/// \brief Resize event.
void RCodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    updateLineGeometry();
}

/// \brief Update the geometry property of line (such as word wrapping)
void RCodeEditor::updateLineGeometry() {
    QRect cr = contentsRect();
    sidebar->setGeometry(QRect(cr.left(),cr.top(),sidebar->sizeHint().width(),cr.height()));
}

/// \brief Update the lineNumberArea width.
void RCodeEditor::updateLineNumberAreaWidth(int) {
    setViewportMargins(sidebar->sizeHint().width(), 0, 0, 0);
}

/// \brief Update the LineNumberArea
void RCodeEditor::updateLineNumberArea(const QRect &rect) {
    sidebar->update(0,rect.y(),sidebar->sizeHint().width(),rect.height());
    updateLineGeometry();
    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

/// \brief Handle the selection.
void RCodeEditor::handleSelectionQuery(const QTextCursor& cursor) {
    auto searchIterator = cursor;
    searchIterator.movePosition(QTextCursor::Start);
    searchIterator = document()->find(cursor.selectedText(), searchIterator);
    while (searchIterator.hasSelection()) {
//        RBorderTextProperty::frame(searchIterator);

        searchIterator = document()->find(cursor.selectedText(), searchIterator);
    }
}

/// \brief Update Extra Selection. (Highlight Current Line and Parentheses)
void RCodeEditor::updateExtraSelection() {
    QList<QTextEdit::ExtraSelection> extra;

    highlightCurrentLine(extra);
    highlightParenthesis(extra);

    setExtraSelections(extra);
}

/// \brief Highlight the parentheses.
void RCodeEditor::highlightParenthesis(QList<QTextEdit::ExtraSelection> &extraSelection) {
    auto currentSymbol = charUnderCursor();
    auto prevSymbol = charUnderCursor(-1);

    for (QPair<QString, QString> &pair: parentheses) {
        int direction;

        QChar counterSymbol;
        QChar activeSymbol;
        int position = textCursor().position();

        if (pair.first == currentSymbol) {
            direction = 1;
            counterSymbol = pair.second[0];
            activeSymbol = currentSymbol;
        } else if (pair.second == prevSymbol) {
            direction = -1;
            counterSymbol = pair.first[0];
            activeSymbol = prevSymbol;
            position--;
        } else {
            continue;
        }

        auto counter = 1;

        while (counter != 0 && position > 0 && position < (document()->characterCount() - 1)) {
            position += direction;

            auto character = document()->characterAt(position);
            if (character == activeSymbol) {
                counter++;
            } else if (character == counterSymbol) {
                counter--;
            }
        }
        QTextCharFormat format = syntaxStyle->getFormat("Parentheses");
        if (counter == 0) {
            QList<QTextEdit::ExtraSelection> extraSelections;
            QTextEdit::ExtraSelection selection;
            QTextCursor cursor = textCursor();
            QTextCharFormat format;

            format.setBackground(Qt::yellow);
            selection.format = format;

            QTextCursor::MoveOperation directionEnum;
            if (direction < 0) {
                directionEnum = QTextCursor::MoveOperation::PreviousCharacter;
            } else {
                directionEnum = QTextCursor::MoveOperation::NextCharacter;
            }

            selection.cursor = cursor;
            selection.cursor.clearSelection();
            selection.cursor.movePosition(directionEnum, QTextCursor::MoveMode::MoveAnchor, std::abs(cursor.position() - position));

            selection.cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, 1);

            extraSelections.append(selection);

            selection.cursor = cursor;
            selection.cursor.clearSelection();
            selection.cursor.movePosition(directionEnum,QTextCursor::MoveMode::KeepAnchor,1);

            extraSelections.append(selection);
            setExtraSelections(extraSelections);
        }

        break;
    }
}

/// \brief Highlight current line.
void RCodeEditor::highlightCurrentLine(QList<QTextEdit::ExtraSelection> &extraSelection) {
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection{};

        selection.format = syntaxStyle->getFormat("CurrentLine");
        selection.format.setForeground(QBrush());
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();

        extraSelection.append(selection);
    }
}

/// \brief Trigger the lineNumberArea repaint.
void RCodeEditor::paintEvent(QPaintEvent *e) {
    updateLineNumberArea(e->rect());
//    updateSidebarWidth();
    QPlainTextEdit::paintEvent(e);
}

/// \brief Get first visible block in current viewport
[[maybe_unused]] int RCodeEditor::getFirstVisibleBlock() {
    QTextCursor curs = QTextCursor(document());
    curs.movePosition(QTextCursor::Start);
    for (int i = 0; i < document()->blockCount(); ++i) {
        QTextBlock block = curs.block();

        QRect r1 = viewport()->geometry();
        QRect r2 = document()->documentLayout()->blockBoundingRect(block).translated(viewport()->geometry().x(),viewport()->geometry().y() - verticalScrollBar()->sliderPosition()).toRect();

        if (r1.intersects(r2)) {
            return i;
        }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

/// \brief Process the completer.
/// \todo  Fix the completer, and add completer rules and AST to analyze the source
bool RCodeEditor::proceedCompleterBegin(QKeyEvent *e) {
    if (pCompleter &&
        pCompleter->popup()->isVisible()) {
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return true; // let the completer do default behavior
            default:
                break;
        }
    }

    auto isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);

    return !(!pCompleter || !isShortcut);

}

/// \brief Process the completer
/// \todo Fix the completer, and add completer rules and AST to analyze the source
void RCodeEditor::proceedCompleterEnd(QKeyEvent *e) {
    auto ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if (!pCompleter ||
        (ctrlOrShift && e->text().isEmpty()) ||
        e->key() == Qt::Key_Delete) {
        return;
    }

    static QString eow(R"(~!@#$%^&*()_+{}|:"<>?,./;'[]\-=)");

    auto isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
    auto completionPrefix = wordUnderCursor();

    if (!isShortcut &&
        (e->text().isEmpty() ||
         completionPrefix.length() < 2 ||
         eow.contains(e->text().right(1)))) {
        pCompleter->popup()->hide();
        return;
    }

    if (completionPrefix != pCompleter->completionPrefix()) {
        pCompleter->setCompletionPrefix(completionPrefix);
        pCompleter->popup()->setCurrentIndex(pCompleter->completionModel()->index(0, 0));
    }

    auto cursRect = cursorRect();
    cursRect.setWidth(
            pCompleter->popup()->sizeHintForColumn(0) +
            pCompleter->popup()->verticalScrollBar()->sizeHint().width()
    );

    pCompleter->complete(cursRect);
}

/// \brief Handle keyPressEvent. Such as autoParentheses, autoIndentation and etc.
void RCodeEditor::keyPressEvent(QKeyEvent *e) {
#if QT_VERSION >= 0x050A00
//    const int defaultIndent = tabStopDistance() / fontMetrics().averageCharWidth();
    const int defaultIndent = 4;
#else
    const int defaultIndent = tabStopWidth() / fontMetrics().averageCharWidth();
#endif

    auto completerSkip = proceedCompleterBegin(e);

    if (!completerSkip) {
        if (pReplaceTab && e->key() == Qt::Key_Tab &&
            e->modifiers() == Qt::NoModifier) {
            insertPlainText(pTabReplace);
            return;
        }

        int indentationLevel = getIndentationSpaces();

#if QT_VERSION >= 0x050A00
        int tabCounts =
                indentationLevel * fontMetrics().averageCharWidth() / tabStopDistance();
#else
        int tabCounts =
        indentationLevel * fontMetrics().averageCharWidth() / tabStopWidth();
#endif
        if (pAutoIndentation &&
            (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) &&
            charUnderCursor() == '}' && charUnderCursor(-1) == '{') {
            if (charUnderCursor() == '{') {
                QTextBlockUserData *data = textCursor().block().userData();

            }
            int charsBack = 0;
            insertPlainText("\n");

            if (pReplaceTab)
                insertPlainText(QString(indentationLevel + defaultIndent, ' '));
            else
                insertPlainText(QString(tabCounts + 1, '\t'));

            insertPlainText("\n");
            charsBack++;

            if (pReplaceTab) {
                insertPlainText(QString(indentationLevel, ' '));
                charsBack += indentationLevel;
            } else {
                insertPlainText(QString(tabCounts, '\t'));
                charsBack += tabCounts;
            }

            while (charsBack--)
                moveCursor(QTextCursor::MoveOperation::Left);
            return;
        }

        if (pReplaceTab && e->key() == Qt::Key_Backtab) {
            indentationLevel = std::min(indentationLevel, static_cast<int>(pTabReplace.size()));
            deIndent(indentationLevel);
            return;
        }

        QPlainTextEdit::keyPressEvent(e);

        if (pAutoIndentation && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
            if (pReplaceTab)
                insertPlainText(QString(indentationLevel, ' '));
            else
                insertPlainText(QString(tabCounts, '\t'));
        }

        if (pAutoParenthese) {
            for (auto &&el: parentheses) {
                if (el.first == e->text()) {
                    insertPlainText(el.second);
                    moveCursor(QTextCursor::MoveOperation::Left);
                    break;
                }

                if (el.second == e->text()) {
                    auto symbol = charUnderCursor();

                    if (symbol == el.second) {
                        textCursor().deletePreviousChar();
                        moveCursor(QTextCursor::MoveOperation::Right);
                    }

                    break;
                }
            }
        }
    }

    proceedCompleterEnd(e);
}

/// \brief Slots, de-indent of current line.
void RCodeEditor::deIndent(int indentationLevel) {
    auto cursor = textCursor();

    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, indentationLevel);

    cursor.removeSelectedText();
}

/// \brief An interface function to give the ability of setting the auto indentation on or off.
void RCodeEditor::setAutoIndentation(bool enabled) {
    pAutoIndentation = enabled;
}

/// \brief An interface function, return the state of codeEditor auto indentation on or off.
bool RCodeEditor::autoIndentation() const {
    return pAutoIndentation;
}

/// \brief An interface function to give the ability of setting the auto parentheses on or off.
void RCodeEditor::setAutoParentheses(bool enabled) {
    pAutoParenthese = enabled;
}

/// \brief An interface function, return the state of setting the auto parentheses on or off.
bool RCodeEditor::autoParentheses() const {
    return pAutoParenthese;
}

/// \brief An interface function to give the ability of setting the tab replacing on or off.
void RCodeEditor::setTabReplace(bool enabled) {
    pReplaceTab = enabled;
}

/// \brief An interface function, return the state of setting the tab replacing on or off.
bool RCodeEditor::tabReplace() const {
    return pReplaceTab;
}

/// \brief An interface function, to set the size of tab replacing.
void RCodeEditor::setTabReplaceSize(int val) {
    pTabReplace.clear();

    pTabReplace.fill(' ', val);
}

/// \brief An interface function, return the value of tabReplaceSize.
int RCodeEditor::tabReplaceSize() const {
    return pTabReplace.size();
}

/// \brief Setup the completer.
void RCodeEditor::setCompleter(QCompleter *completer) {
    if (pCompleter) {
        disconnect(pCompleter, nullptr, this, nullptr);
    }

    pCompleter = completer;

    if (!pCompleter) {
        return;
    }

    pCompleter->setWidget(this);
    pCompleter->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);

    connect(
            pCompleter,
            QOverload<const QString &>::of(&QCompleter::activated),
            this,
            &RCodeEditor::insertCompletion
    );
}

/// \brief Focus In Event, an override version of factory function.
void RCodeEditor::focusInEvent(QFocusEvent *e) {
    if (pCompleter) {
        pCompleter->setWidget(this);
    }

    QPlainTextEdit::focusInEvent(e);
}

/// \brief Insert Completion.
void RCodeEditor::insertCompletion(QString s) {
    if (pCompleter->widget() != this) {
        return;
    }

    auto tc = textCursor();
    tc.select(QTextCursor::SelectionType::WordUnderCursor);
    tc.insertText(s);
    setTextCursor(tc);
}

/// \brief An interface function, return the object of completer;
QCompleter *RCodeEditor::completer() const {
    return pCompleter;
}

/// \brief Return the character of the cursor, depends on offset is left (negative) or right (positive)
QChar RCodeEditor::charUnderCursor(int offset) const {
    auto block = textCursor().blockNumber();
    auto index = textCursor().positionInBlock();
    auto text = document()->findBlockByNumber(block).text();

    index += offset;

    if (index < 0 || index >= text.size()) {
        return {};
    }

    return text[index];
}

/// \brief Return the string(word) of the cursor. NOTE: NO OFFSET ALLOWED
QString RCodeEditor::wordUnderCursor() const {
    auto tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

/// \brief Insert the code from Mime Data.
void RCodeEditor::insertFromMimeData(const QMimeData *source) {
    insertPlainText(source->text());
}

/// \brief Get the indentation spaces of current line.
int RCodeEditor::getIndentationSpaces() {
    auto blockText = textCursor().block().text();

    int indentationLevel = 0;

    for (auto i = 0;
         i < blockText.size() && QString("\t ").contains(blockText[i]);
         ++i) {
        if (blockText[i] == ' ') {
            indentationLevel++;
        } else {
#if QT_VERSION >= 0x050A00
            indentationLevel += tabStopDistance() / fontMetrics().averageCharWidth();
#else
            indentationLevel += tabStopWidth() / fontMetrics().averageCharWidth();
#endif
        }
    }

    return indentationLevel;
}

/// \brief Slot, trigger folding or not.
void RCodeEditor::fold() {
//    foldingHandler::fold(textCursor());
}

/// \brief Slot, trigger unfloding or not.
void RCodeEditor::unfold() {
//    foldingHandler::unfold(textCursor());
}

/// \brief Retrigger the sidebarPaintEvent.
void RCodeEditor::sidebarPaintEvent(QPaintEvent *pEvent) {
    QPainter painter(sidebar);
    painter.fillRect(pEvent->rect(), QColor("#282a36"));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= pEvent->rect().bottom()) {
        if (block.isVisible() && bottom >= pEvent->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.drawText(0, top, sidebar->width(), fontMetrics().height(), Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }

    const int lineLength = sidebar->width();
    const int lineHeight = fontMetrics().height();
    const int yPosition = sidebar->height() / 2;
    painter.setPen(Qt::white);
    painter.drawLine(lineLength - 1, 0, lineLength - 1, sidebar->height());


}

/// \brief Return the width of sidebar.
int RCodeEditor::sidebarWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = Reto_SideBar_Default_Width + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}