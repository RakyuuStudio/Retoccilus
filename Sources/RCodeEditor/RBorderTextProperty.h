#ifndef RetoCodeEditor_BorderTextProperty_h
#define RetoCodeEditor_BorderTextProperty_h

#pragma once

#include <QObject>
#include <QPainter>
#include "RSyntaxStyle.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QTextObjectInterface>

class RSyntaxStyle;

class RBorderTextProperty : public QObject, public QTextObjectInterface {
Q_OBJECT

    Q_INTERFACES(QTextObjectInterface)
public:
    enum Property {
        FramedString = 1
    };
    static int type() { return 1 + QTextFormat::UserFormat; }
    explicit RBorderTextProperty(QObject *parent = nullptr) : QObject(parent), rcStyle(nullptr) {}
    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format) { return {0, 0}; }
    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument,
                    const QTextFormat &format)
    {
        auto textCharFormat = reinterpret_cast<const QTextCharFormat&>(format);
        QFont font = textCharFormat.font();
        QFontMetrics metrics(font);
        QString string = format.property(FramedString).toString();
        QSize stringSize = metrics.boundingRect(string).size();
        QRectF drawRect(rect.topLeft(), stringSize);
        drawRect.moveTop(rect.top() - stringSize.height());
        drawRect.adjust(0, 4, 0, 4);
        painter->setPen(rcStyle->getFormat("Occurrences").background().color());
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRoundedRect(drawRect, 4, 4);
    }
    static void frame(QTextCursor cursor) {
        QTextCharFormat format;
        format.setObjectType(type());
        format.setProperty(FramedString, cursor.selectedText());
        if (cursor.selectionEnd() > cursor.selectionStart()) {
            cursor.setPosition(cursor.selectionStart());
        } else {
            cursor.setPosition(cursor.selectionEnd());
        }
        cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
    }
    static void clear(QTextCursor cursor) {
        QTextDocument *doc = cursor.document();
        for (int blockIndex = 0; blockIndex < doc->blockCount(); blockIndex++) {
            QTextBlock block = doc->findBlockByNumber(blockIndex);
            auto formats = block.textFormats();
            int offset = 0;
            for (auto &format: formats) {
                if (format.format.objectType() == type()) {
                    cursor.setPosition(block.position() + format.start - offset);
                    cursor.deleteChar();
                    offset++;
                }
            }
        }
    }
    void setSyntaxStyle(RSyntaxStyle *style) { rcStyle = style; }
    [[nodiscard]] RSyntaxStyle *syntaxStyle() const { return rcStyle; }
private:
    RSyntaxStyle *rcStyle;
};

#endif