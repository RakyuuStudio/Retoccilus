#ifndef RCodeEditor_Sidebar_h
#define RCodeEditor_Sidebar_h

#pragma once

#include "RSyntaxStyle.h"
#include "RCodeEditor.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QTextEdit>

class RCodeEditor; //Fuck you forward declaration

class RSideBar : public QWidget {
Q_OBJECT

public:
    explicit RSideBar(RCodeEditor *parent = nullptr)
            : pSyntaxStyle(nullptr), codeEditorP(parent) {

    }

    [[nodiscard]] QSize sizeHint() const override {
        if (codeEditorP == nullptr) {
            return QWidget::sizeHint();
        }

        int digits = 1;
        int max = qMax(1, codeEditorP->document()->blockCount());
        while (max >= 10) {
            max /= 10;
            digits++;
        }

#if QT_VERSION >= 0x050B00
        int space = 13 + codeEditorP->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#else
        int space = 13 + codeEditorP->fontMetrics().width(QLatin1Char('9')) * digits;
#endif

        return {space, 0};
    }

    void setSyntaxStyle(RSyntaxStyle *style) {
        pSyntaxStyle = style;
    }

    [[nodiscard]] RSyntaxStyle *syntaxStyle() const {
        return pSyntaxStyle;
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.fillRect(event->rect(), pSyntaxStyle->getFormat("Text").background().color());

        auto blockNumber = codeEditorP->getFirstVisibleBlock();
        auto block = codeEditorP->document()->findBlockByNumber(blockNumber);
        auto top = (int) codeEditorP->document()->documentLayout()->blockBoundingRect(block).translated(0, -codeEditorP->verticalScrollBar()->value()).top();
        auto bottom = top + (int) codeEditorP->document()->documentLayout()->blockBoundingRect(block).height();

        auto currentLine = pSyntaxStyle->getFormat("CurrentLineNumber").foreground().color();
        auto otherLines = pSyntaxStyle->getFormat("LineNumber").foreground().color();

        painter.setFont(codeEditorP->font());

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1);

                bool isCurrentLine = codeEditorP->textCursor().blockNumber() == blockNumber;
                if (isCurrentLine) {
//                    painter.setPen(currentLine);
                    painter.setPen(QColor("#ffffff"));
                } else {
                    painter.setPen(otherLines);
                }
                painter.drawText(-5, top, sizeHint().width(), codeEditorP->fontMetrics().height(), Qt::AlignRight,
                                 number);
            }

            block = block.next();
            top = bottom;
            bottom = top + (int) codeEditorP->document()->documentLayout()->blockBoundingRect(block).height();
            blockNumber++;
        }
    }

private:
    RSyntaxStyle *pSyntaxStyle;
    RCodeEditor *codeEditorP;
};

#endif