#ifndef RetoCodeEditor_h
#define RetoCodeEditor_h

#include <QPlainTextEdit>
#include <QChar>
#include <QFont>

namespace Retoccilus {
    static QVector<QPair<QString, QString>> parentheses = {
            {"(", ")"},
            {"{", "}"},
            {"[", "]"},
            {"\"", "\""},
            {"'", "'"}
    };
}

#endif //RetoCodeEditor_h