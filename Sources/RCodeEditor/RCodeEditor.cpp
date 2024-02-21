#include <QPainter>
#include <QTextBlock>
#include "RCodeEditor.h"
#include "RetoCodeEditor.h"
#include "RSyntaxHighlighter.h"

using namespace RetoUtils;

RCodeEditor::RCodeEditor(QWidget *widget) :
        QPlainTextEdit(widget), autoIndentation(true), replaceTab(true), autoParenthese(true) {
    sideBarArea = new SidebarArea(this);

    connect(this, &RCodeEditor::blockCountChanged, this, &RCodeEditor::updateSideBarAreaWidth);
    connect(this, &RCodeEditor::updateRequest, this, &RCodeEditor::updateSideBarArea);
    connect(this, &RCodeEditor::cursorPositionChanged, this, &RCodeEditor::highlightCurrentLine);

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
    //I DON'T KNOW WHAT THE HECK THE "newBlockCount" VARIABLE EXIST
    //BECAUSE THIS VARIABLE IS NEED TO MATCH THE PARAMETER LIST IN CONNECT FUNCTION HOLY FUCK
    Q_UNUSED(newBlockCount); //I DON'T NEED YOU MORE, This is literally unnecessary
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
    //==Process Auto Pair==
    if (event->text() == "(") {
        insertPlainText(")");
        moveCursor(QTextCursor::MoveOperation::Left);
    }
    else if (event->text() == ")") {
        QString lc;
        moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
        lc = textCursor().selectedText();
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
        if (lc == "(") {
            moveCursor(QTextCursor::MoveOperation::Right);
            return;
        }
        else {
            insertPlainText(")");
        }
    }
    else if (event->text() == "[") {
        insertPlainText("]");
        moveCursor(QTextCursor::MoveOperation::Left);
    }
    else if (event->text() == "]") {
        QString lc;
        moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
        lc = textCursor().selectedText();
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
        if (lc == "[") {
            moveCursor(QTextCursor::MoveOperation::Right);
            return;
        }
        else {
            insertPlainText("]");
        }
    }
    else if (event->text() == "{") {
        insertPlainText("}");
        moveCursor(QTextCursor::MoveOperation::Left);
    }
    else if (event->text() == "}") {
        QString lc;
        moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
        lc = textCursor().selectedText();
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
        if (lc == "{") {
            moveCursor(QTextCursor::MoveOperation::Right);
            return;
        }
        else {
            insertPlainText("}");
        }
    }
    else if (event->text() == "'") {
        insertPlainText("'");
        moveCursor(QTextCursor::MoveOperation::Left);
    }
    else if (event->text() == "'") {
        QString lc;
        moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
        lc = textCursor().selectedText();
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
        if (lc == "'") {
            moveCursor(QTextCursor::MoveOperation::Right);
            return;
        }
        else {
            insertPlainText("'");
        }
    }else if (event->text() == "\"") {
        insertPlainText("\"");
        moveCursor(QTextCursor::MoveOperation::Left);
    }
    else if (event->text() == "\"") {
        QString lc;
        moveCursor(QTextCursor::Left, QTextCursor::KeepAnchor);
        lc = textCursor().selectedText();
        moveCursor(QTextCursor::Right, QTextCursor::MoveAnchor);
        if (lc == "\"") {
            moveCursor(QTextCursor::MoveOperation::Right);
            return;
        }
        else {
            insertPlainText("\"");
        }
    }
    QPlainTextEdit::keyPressEvent(event);
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
    auto blockText = textCursor().block().text(); //Clang-Tidy recommend me
    int indentationLevel = 0;

    for (int i = 0; i < blockText.size() && QString("\t ").contains(blockText[i]); i++) {
        if (blockText[i] == ' ') {
            indentationLevel++;
        }
        else {
#if QT_VERSION >= 0x050A00
            //If Qt major version >= 5 (Qt 5.0.0 above)
            const int defaultIndent = tabStopDistance() / fontMetrics().averageCharWidth();
#else
            const int defaultIndent = tabtStopWidth() / fontMetrics().averageCharWidth();
#endif
        }
    }

    return indentationLevel;
}