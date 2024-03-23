//==-- RCFamilyHighlighter.h - RCodeEditor C-Family Highlighter Header----*- C++ -*-==//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.                      *//
//* Retoccilus licensed under GNU GPL v3 license.                                    *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.           *//
//==-----------------------------------------------------------------------------====*//

#ifndef RetoCodeEditor_CFamilyHighlighter_h
#define RetoCodeEditor_CFamilyHighlighter_h

#pragma once
#include "RLanguage.h"
#include "RStyleSyntaxHighlighter.h"
#include "RHighlightRule.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QVector>
#include <QFile>
#include <utility>
class RSyntaxStyle;
class RCFamilyHighlighter : public QSyntaxHighlighter {
public:
    explicit RCFamilyHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
        keywordFormat.setForeground(QColor(255, 108, 133));
        keywordFormat.setFontItalic(true);
        keywordFormat.setFontWeight(QFont::Bold);
        singleLineCommentFormat.setFontItalic(true);
        singleLineCommentFormat.setForeground(QColor(138, 174, 255));
        multilineCommentFormat.setForeground(QColor(138, 174, 255).darker(100));
        multilineCommentFormat.setFontItalic(true);
        preprocessorFormat.setForeground(QColor(124, 130, 248).lighter(100));
        parentheseFormat.setForeground(QColor("#fff906"));
        bracketFormat.setForeground(QColor("#33ffab"));
        braceFormat.setForeground(QColor("#5b7eff"));
        quotationFormat.setForeground(QColor("#eaf36f"));
        apostropheFormat.setForeground(QColor("#d7f98b"));
        rawStrLitFormat.setForeground(QColor(146, 146, 248));
        identifierFormat.setForeground(QColor("#8be9fd"));
        functionFormat.setForeground(QColor("#4ef579"));
        numberFormat.setForeground(QColor("#9292f8"));
        QStringList keywordsL;
        keywordsL = readKeywords(":/config/Configuration/RCodeEditor/KeywordList/cppKeywords.xml");
        for (const QString &pattern : keywordsL) {
            highlightingRules.append({QRegularExpression("\\b" + pattern + "\\b"), keywordFormat});
        }
        singleLineCommentExp.setPattern("//[^\n]*");
        commentStartExp.setPattern("/\\*");
        commentEndExp.setPattern("\\*/");
        preprocessExp.setPattern("#[^\\s]*");
        highlightingRules.append({QRegularExpression("\\("), parentheseFormat});
        highlightingRules.append({QRegularExpression("\\)"), parentheseFormat});
        highlightingRules.append({QRegularExpression("\\["), bracketFormat});
        highlightingRules.append({QRegularExpression("\\]"), bracketFormat});
        highlightingRules.append({QRegularExpression("\\{"), braceFormat});
        highlightingRules.append({QRegularExpression("\\}"), braceFormat});
        highlightingRules.append({QRegularExpression("'[^']*'"), apostropheFormat});
        highlightingRules.append({QRegularExpression(R"("[^"]*")"), quotationFormat});
        highlightingRules.append({QRegularExpression("R\"(.*?\\n?)\""), rawStrLitFormat});
        highlightingRules.append({QRegularExpression(R"(\\b[A-Za-z_]+[A-Za-z0-9_]*\\b)"), identifierFormat});
        highlightingRules.append({QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()"), functionFormat});
        highlightingRules.append({QRegularExpression("^(-)?(0b[01]+|0[0-7]+|0x[0-9A-Fa-f]+|\\d+)$"), numberFormat});
    }
    QStringList readKeywords(const QString &filePath) {
        QStringList names;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open XML file";
            return names;
        }
        QXmlStreamReader xmlReader(&file);
        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if (token == QXmlStreamReader::StartElement && xmlReader.name().toString() == "name") {
                QString name = xmlReader.readElementText();
                names.append(name);
            }
        }
        if (xmlReader.hasError()) {
            qWarning() << "XML parsing error:" << xmlReader.errorString();
        }
        file.close();
        return names;
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
        QRegularExpressionMatch preprocessorMatch = preprocessExp.match(text);
        if (preprocessorMatch.hasMatch()) {
            setFormat(preprocessorMatch.capturedStart(), preprocessorMatch.capturedEnd(), preprocessorFormat);
        }
        QRegularExpressionMatch singleLineCommentMatch = singleLineCommentExp.match(text);
        if (singleLineCommentMatch.hasMatch()) {
            setFormat(singleLineCommentMatch.capturedStart(), singleLineCommentMatch.capturedLength(), singleLineCommentFormat);
        }
        setCurrentBlockState(0);
        int startIndex = 0;
        if (previousBlockState() != 1) {
            startIndex = text.indexOf(commentStartExp);
        }
        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExp.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multilineCommentFormat);
            startIndex = text.indexOf(commentStartExp, startIndex + commentLength);
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
    QRegularExpression singleLineCommentExp;
    QRegularExpression preprocessExp;
    QRegularExpression braceExp, bracketExp, parentheseExp;
    QRegularExpression apostropheExp, quotationExp;
    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multilineCommentFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat braceFormat, bracketFormat, parentheseFormat;
    QTextCharFormat apostropheFormat, quotationFormat, rawStrLitFormat;
    QTextCharFormat identifierFormat, functionFormat;
    QTextCharFormat numberFormat;
};
#endif //RetoCodeEditor_CFamilyHighlighter_h