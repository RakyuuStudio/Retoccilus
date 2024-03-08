#include "foldingHandler.h"
#include <QTextDocumentFragment>

foldingHandler::foldingHandler(QObject *parent) : QObject(parent) {

}

QSizeF foldingHandler::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format) {
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    Q_ASSERT(format.type() == format.CharFormat);
    const auto &tf = reinterpret_cast<const QTextCharFormat &>(format);

    QFont fn = tf.font();
    QFontMetrics fm(fn);

    QString s("...");
    QSizeF sz = fm.boundingRect(s).size();
    return sz;
}

void foldingHandler::drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument,
                                const QTextFormat &format) {
    Q_UNUSED(doc)
    Q_UNUSED(posInDocument)
    Q_ASSERT(format.type() == format.CharFormat);
    QString s("/*...*/");
    painter->drawText(rect, s);
    painter->drawRect(rect);
}

void foldingHandler::fold(QTextCursor c) {
    QTextCharFormat f;
    f.setObjectType(type());
    QVariant v;
    v.setValue(c.selection());
    f.setProperty(prop(), v);
    c.insertText(tr("/*.Folded.*/"), f);
}

bool foldingHandler::unfold(QTextCursor c) {
    if (!c.hasSelection()) {
        QTextCharFormat f = c.charFormat();
        if (f.objectType() == type()) {
            c.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            QVariant v = f.property(prop());
            auto q = v.value<QTextDocumentFragment>();
            c.insertFragment(q);
            return true;
        }
    }
    return false;
}