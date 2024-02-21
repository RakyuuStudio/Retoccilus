#ifndef RSyntaxHighlighter_h
#define RSyntaxHighlighter_h

/*
 * Keyword: #ff6c85                   255, 108, 133
 * Comment: #8aaeff                   138, 174, 255
 * Preprocessor: #7c82f8              124, 130, 248
 * Header File Quotation: #f1fa8c     241, 250, 140
 * Used Identifier: #4ff966           241, 250, 140
 * Class: #5ecdfc                     94,  205, 252
 * Symbols: #fff906                   255, 249, 6
 * Numbers: #9292f8                   146, 146, 248
 * Normal Characters: #ffffff         255, 255, 255
 */

#include <QTextDocument>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

#include "RXmlReader.h"

using namespace RetoUtils;

class RSyntaxHighlighter : public QSyntaxHighlighter {
public:
    RSyntaxHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        keywordFormat.setForeground(QColor(255, 108, 133));
        keywordFormat.setFontItalic(true);
        keywordFormat.setFontWeight(QFont::Bold);

        commentFormat.setFontItalic(true);
        commentFormat.setForeground(QColor(138, 174, 255));

        QStringList keywordsL;
        keywordsL = xmlr.readKeywords(":/config/Configuration/RCodeEditor/KeywordList/cppKeywords.xml");

        for (const QString &pattern : keywordsL) {
            highlightingRules.append({QRegularExpression("\\b" + pattern + "\\b"), keywordFormat});
        }

        commentStartExp.setPattern("//[^\n]*");
    }
protected:
    void highlightBlock(const QString &text) override {
        for (const HighlightingRule &rule : highlightingRules) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }

        int commentStart = text.indexOf(commentStartExp);
        if (commentStart >= 0) {
            int commentEnd = text.indexOf('\n', commentStart);
            setFormat(commentStart, commentEnd - commentStart, commentFormat);
        }
    }
private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
    QRegularExpression commentStartExp;
    QRegularExpression commentEndExp;

    QTextCharFormat keywordFormat;
    QTextCharFormat commentFormat;

    XmlReader xmlr;
};

#endif //RSyntaxHighlighter_h