#ifndef RetoCodeEditor_foldingHandler_h
#define RetoCodeEditor_foldingHandler_h

#include <QObject>
#include <QPainter>
#include <QTextDocument>
#include <QTextObjectInterface>
#include <QTextObject>

#include <functional>

class foldingHandler : public QObject, public QTextObjectInterface {
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)

public:
    explicit foldingHandler(QObject *parent);
    static int type() {
        return QTextFormat::UserObject + 2;
    }

    static int prop() {
        return 2;
    }

    struct foldingStructure {
        QString foldingContent;
        int foldingRows;
    };

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format) override;
    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format) override;
    static foldingStructure fold(QTextCursor c);
    static bool unfold(QTextCursor c, foldingStructure fs);
};

#endif