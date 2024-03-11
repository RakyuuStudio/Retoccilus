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

foldingHandler::foldingStructure foldingHandler::fold(QTextCursor c) {
    foldingStructure fs;
    QTextCharFormat format;
    format.setObjectType(type());
    QVariant v;
    v.setValue(c.selection());

    fs.foldingContent = c.selectedText();
    fs.foldingRows = static_cast<int>(c.selectedText().count("\n"));

    format.setProperty(prop(), v);
    c.insertText(tr("/*.Folded.*/"), format);
    return fs;
}

bool foldingHandler::unfold(QTextCursor cursor, foldingStructure fs) {
    if (!cursor.hasSelection()) {
        QTextCharFormat format = cursor.charFormat();
        if (format.objectType() == type()) {
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
            QVariant variant = format.property(prop());
//            QTextDocumentFragment fragment = variant.value<QTextDocumentFragment>();
            cursor.insertText(fs.foldingContent);
            fs.foldingContent = "";
            fs.foldingRows = 0;
            return true;
        }
    }
    return false;
}

