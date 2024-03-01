#include <QPainter>
#include <QTextBlock>
#include "RCodeEditor.h"
#include "RetoCodeEditor.h"
#include "RSyntaxHighlighter.h"

using namespace Retoccilus;

RCodeEditor::RCodeEditor(QWidget *widget) :
        QPlainTextEdit(widget), autoIndentation(true), replaceTab(true), autoParenthese(true), indentLayer(0) {
    sideBarArea = new SidebarArea(this);

    connect(this, &RCodeEditor::blockCountChanged, this, &RCodeEditor::updateSideBarAreaWidth);
    connect(this, &RCodeEditor::updateRequest, this, &RCodeEditor::updateSideBarArea);
    connect(this, &RCodeEditor::cursorPositionChanged, this, &RCodeEditor::highlightCurrentLine);

    indentLayer = 0;

    updateSideBarAreaWidth(0);
    highlightCurrentLine();
}

RCodeEditor::~RCodeEditor() = default;

int RCodeEditor::getFirstVisibleBlock() {
    return 0;
}

bool RCodeEditor::autoIndent() const {
    return autoIndentation;
}

void RCodeEditor::setIndentation(bool i) {
    if (i) {
        autoIndentation = true;
    } else {
        autoIndentation = false;
    }
}

void RCodeEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    QRect currentRect = contentsRect();
    sideBarArea->setGeometry(QRect(currentRect.left(), currentRect.top(), sideBarAreaWidth(), currentRect.height()));
}

void RCodeEditor::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y();
        double fSize = font().pointSize();
        QFont font = this->font();

        if (delta > 0) {
            font.setPointSize(font.pointSizeF() + 1.0);

            QFont lineNumberFont = sideBarArea->font();
            lineNumberFont.setPointSizeF(lineNumberFont.pointSizeF() + 1.0);
            sideBarArea->setFont(lineNumberFont);
        } else if (delta < 0) {
            font.setPointSizeF(font.pointSizeF() - 1.0);

            QFont lineNumberFont = sideBarArea->font();
            lineNumberFont.setPointSizeF(lineNumberFont.pointSizeF() - 1.0);
            sideBarArea->setFont(lineNumberFont);
        }

        this->setFont(font);
        event->accept();
    } else {
        QPlainTextEdit::wheelEvent(event);
    }
}


void RCodeEditor::sideBarAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(sideBarArea);
    painter.fillRect(event->rect(), QColor(57, 59, 90));

    QFont lineNumberFont;
    lineNumberFont.setFamily("Consolas");
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::white);
            painter.setFont(lineNumberFont);
            painter.drawText(0, top, sideBarArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        blockNumber++;
    }
}

int RCodeEditor::sideBarAreaWidth() {
    int digit = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        digit++;
    }
    int space = 9 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digit * 2;
    return space;
}

void RCodeEditor::updateSideBarAreaWidth(int newBlockCount) {
    Q_UNUSED(newBlockCount);
    setViewportMargins(sideBarAreaWidth(), 0, 0, 0);
}

void RCodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::white).lighter(30);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void RCodeEditor::updateSideBarArea(const QRect &rect, int dy) {
    if (dy) {
        //If dy exists (means it is not zero)
        sideBarArea->scroll(0, dy);
    } else {
        sideBarArea->update(0, rect.y(), sideBarArea->width(), rect.height());
    }
    if (rect.contains(viewport()->rect())) {
        updateSideBarAreaWidth(0);
    }
}


void RCodeEditor::keyPressEvent(QKeyEvent *event) {
//    indentLayer = 0;
    if (event->key() == '(') {
        insertPlainText("()");
        moveCursor(QTextCursor::PreviousCharacter);
    } else if (event->key() == '{') {
        insertPlainText("{}");
        moveCursor(QTextCursor::PreviousCharacter);
    } else if (event->key() == '[') {
        insertPlainText("[]");
        moveCursor(QTextCursor::PreviousCharacter);
    } else if (event->key() == '\'') {
        if (charUnderCursor(-1) == '\'') {
            moveCursor(QTextCursor::NextCharacter);
        } else {
            insertPlainText("''");
            moveCursor(QTextCursor::PreviousCharacter);
        }
    } else if (event->key() == '\"') {
        if (charUnderCursor(-1) == '\"') {
            moveCursor(QTextCursor::NextCharacter);
        } else {
            insertPlainText("\"\"");
            moveCursor(QTextCursor::PreviousCharacter);
        }
    } else if (event->key() == ')' && charUnderCursor() == ')') {
        moveCursor(QTextCursor::NextCharacter);
    } else if (event->key() == '}' && charUnderCursor() == '}') {
        moveCursor(QTextCursor::NextCharacter);
    } else if (event->key() == ']' && charUnderCursor() == ']') {
        moveCursor(QTextCursor::NextCharacter);
    }
    else if (event->key() == Qt::Key_Return) {
        if (charUnderCursor(-1) == '{') {
            qDebug() << indentLayer;
            insertPlainText("\n");
            if (autoIndentation) {
                if (spacesFront() > 0) {
                    insertPlainText(QString(countSpacesAtCurrentLine(getIndentationSpaces()) * indentLayer, ' '));
                } else {
                    indentLayer++;
                    QTextCursor cursor = textCursor();
                    cursor.movePosition(QTextCursor::StartOfBlock);
                    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                    QString text = cursor.selectedText();
                    int spaces = countSpacesAtCurrentLine(getIndentationSpaces());
                    if (text.endsWith('{')) {
                        spaces += defaultIndent;
                    }
                    insertPlainText("\n");
                    cursor.movePosition(QTextCursor::Up);
                    setTextCursor(cursor);
                    update();
                    if (indentLayer > 1) {
                        qDebug() << "YES";
                        qDebug() << indentLayer;
//                        insertPlainText(QString(4 * indentLayer, ' '));
                        cursor.movePosition(QTextCursor::Down);
                        cursor.movePosition(QTextCursor::StartOfLine);
                        setTextCursor(cursor);
                        update();
                        cursor.insertText(QString(4 * (indentLayer - 1), ' '));
                        cursor.movePosition(QTextCursor::Up);
                        setTextCursor(cursor);
                        update();
                        insertPlainText(QString(4 * indentLayer, ' '));
                    } else if (indentLayer == 1) {
                        qDebug() << "NO";
                        insertPlainText(QString(4 * indentLayer, ' '));
                    }
                    setTextCursor(cursor);
                    update();
                }
            }
        }
        else if (charUnderCursor(-1) == '}') {
            indentLayer--;
            insertPlainText("\n");
            if (autoIndentation) {
                QTextCursor cursor = textCursor();
                cursor.movePosition(QTextCursor::StartOfBlock);
                cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                QString text = cursor.selectedText();
                int spaces = countSpacesAtCurrentLine(getIndentationSpaces());
                if (text.endsWith('{')) {
                    spaces += defaultIndent;
                }
                insertPlainText("\n");
                cursor.movePosition(QTextCursor::Up);
                setTextCursor(cursor);
                update();
                insertPlainText(QString(4 * indentLayer, ' '));
            }
        }
        else {
            insertPlainText("\n");
//            if (autoIndentation) {
//                insertPlainText(QString(4 * indentLayer, ' '));
//            }
        }
    }
    else {
        QPlainTextEdit::keyPressEvent(event);
    }
}

QChar RCodeEditor::charUnderCursor(int offset) const {
    int block = textCursor().blockNumber();
    auto index = textCursor().positionInBlock();
    auto text = document()->findBlockByNumber(block).text();

    index += offset;
    if (index < 0 || index >= text.size()) {
        return {}; //Return an empty set
    }

    return text[index];
}

int RCodeEditor::getIndentationSpaces() {
    int indentationLevel = 0;
    QTextBlock block = textCursor().block();
    while (block.isValid()) {
        QString text = block.text();
        for (QChar c : text) {
            if (c == ' ') {
                indentationLevel++;
            } else if (c == '\t') {
                indentationLevel += defaultIndent;
            } else {
                return indentationLevel / defaultIndent;
            }
        }
        block = block.previous();
    }
    return 0;
}

int RCodeEditor::countSpacesAtCurrentLine(int indentationLevel) {
    QTextBlock block = textCursor().block();
    QString text = block.text();
    int spaces = 0;
    for (QChar c : text) {
        if (c == ' ') {
            spaces++;
        } else if (c == '\t') {
            spaces += defaultIndent;
        } else {
            break;
        }
    }
    return spaces - indentationLevel * defaultIndent;
}

int RCodeEditor::spacesFront() {
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    int spaces = 0;
    while (true) {
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        if (cursor.selectedText() == " ") {
            spaces++;
        }
        else {
            break;
        }
    }
    return spaces;
}