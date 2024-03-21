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
#include <RDefs.h>

//namespace RetoccilusCodeEditor {
    class RCFamilyHighlighter : public QSyntaxHighlighter {
    public:
        explicit RCFamilyHighlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {
            initRegexp(); //Initialize Regular Expressions
            readAndSetStyle(":/config/Configuration/RCodeEditor/ColorThemes/Dracula.xml"); //Read and set default color theme

            QStringList keywordsL;
            keywordsL = readKeywords(":/config/Configuration/RCodeEditor/KeywordList/cppKeywords.xml");

//            QStringList doxygenTagsList;
//            doxygenTagsList = readKeywords(":/config/Configuration/RCodeEditor/KeywordList/doxygenTags.xml");

            for (const QString &pattern: keywordsL) {
                highlightingRules.append({QRegularExpression("\\b" + pattern + "\\b"), keywordFormat});
            }

//            for (const QString &pattern: doxygenTagsList) {
//                highlightingRules.append({QRegularExpression("\\b" + pattern + "\\b"), keywordFormat});
//            }
            highlightingRules.append({QRegularExpression("\\("), parentheseFormat});
            highlightingRules.append({QRegularExpression("\\)"), parentheseFormat});
            highlightingRules.append({QRegularExpression("\\["), bracketFormat});
            highlightingRules.append({QRegularExpression("\\]"), bracketFormat});
            highlightingRules.append({QRegularExpression("\\{"), braceFormat});
            highlightingRules.append({QRegularExpression("\\}"), braceFormat});
            highlightingRules.append({numberDecExp, NumberFormat});
            highlightingRules.append({numberHexExp, NumberFormat});
            highlightingRules.append({numberOctExp, NumberFormat});
            highlightingRules.append({numberBinExp, NumberFormat});
            highlightingRules.append({numberFloatExp, NumberFormat});
            highlightingRules.append({stringApostropheExp, stringFormat});
            highlightingRules.append({stringQuotationExp, stringFormat});
            highlightingRules.append({stringRawLiteralExp, stringFormat});
            highlightingRules.append({identifierExp, identifierFormat});
            highlightingRules.append({commentStartExp, commentFormat});
            highlightingRules.append({commentEndExp, commentFormat});
            highlightingRules.append({preprocessExp, preprocessorFormat});
        }

        QStringList readKeywords(const QString &filePath) {
            QStringList names;

            QFile file(filePath);
//            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//                qDebug() << "Value of File Path" << filePath;
//                qDebug() << "Failed to open XML file：" << file.errorString();
//                return names;
//            }
            qDebug() << "AAh" << filePath;
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

        void readAndSetStyle(const QString& colorThemeXMLPath) {
            QFile colorSchemeFile(colorThemeXMLPath);
            if (!colorSchemeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qWarning() << "Failed to open XML file";
                return;
            }

            QXmlStreamReader xmlReader(&colorSchemeFile);
            while (!xmlReader.atEnd() && !xmlReader.hasError()) {
                QXmlStreamReader::TokenType token = xmlReader.readNext();
                if (token == QXmlStreamReader::StartElement && xmlReader.name().toString() == "color") {
                    QString name = xmlReader.attributes().value("name").toString();
//                    QString value = xmlReader.readElementText();
                    if (name == "Keyword") {
                        keywordFormat.setForeground(QColor(xmlReader.attributes().value("value").toString()));
                        keywordFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                        if (xmlReader.attributes().value("bold").toString() == "true") {
                            keywordFormat.setFontWeight(QFont::Bold);
                        }
                        if (xmlReader.attributes().value("italic").toString() == "true") {
                            keywordFormat.setFontItalic(true);
                        }
                    }
                    else if (name == "Selection") {
                        selectionFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                    }
                    else if (name == "SearchResult") {
                        searchResultFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        searchResultFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "Parentheses") {
                        parentheseFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        parentheseFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "CurrentLine") {
                        currentLineFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        currentLineFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                    }
                    else if (name == "CurrentLineNumber") {
                        currentLineNumberFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        currentLineNumberFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                    }
                    else if (name == "Occurrences") {
                        OccurrencesFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "OccurrencesUnused") {
                        UnusedOccurrencesFormat.setUnderlineColor(QColor(xmlReader.attributes().value("underlineColor").toString()));
                        UnusedOccurrencesFormat.setUnderlineStyle(xmlReader.attributes().value("underlineStyle").toString() == "Single" ? QTextCharFormat::SingleUnderline : QTextCharFormat::NoUnderline);
                    }
                    else if (name == "Number") {
                        NumberFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "String") {
                        stringFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Type") {
                        typeFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "VirtualMethod") {
                        virtualMethodFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        virtualMethodFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                        if (xmlReader.attributes().value("italic").toString() == "true") {
                            virtualMethodFormat.setFontItalic(true);
                        }
                    }
                    else if (name == "Function") {
                        functionFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        functionFormat.setBackground(QColor(xmlReader.attributes().value("background").toString()));
                    }
                    else if (name == "PrimitiveType") {
                        primitiveTypeFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Operator") {
                        operatorFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Preprocessor") {
                        preprocessorFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Identifier") {
                        identifierFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Comment") {
                        commentFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "DoxygenCommentTag") {
                        DoxygenCommentTagFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Property") {
                        propertyFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                    else if (name == "Warning") {
                        warningFormat.setUnderlineColor(QColor(xmlReader.attributes().value("underlineColor").toString()));
                    }
                    else if (name == "Error") {
                        errorFormat.setUnderlineColor(QColor(xmlReader.attributes().value("underlineColor").toString()));
                    }
                    else if (name == "ErrorContext") {
                        errorContextFormat.setForeground(QColor(xmlReader.attributes().value("underlineColor").toString()));
                    }
                    else if (name == "Declaration") {
                        declarationFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "Brace") {
                        braceFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        braceFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "Bracket") {
                        bracketFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        bracketFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "Parentheses") {
                        parentheseFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        parentheseFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "QuotationMark") {
                        quotationFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        quotationFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "Apostrophe") {
                        apostropheFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                        apostropheFormat.setFontWeight(xmlReader.attributes().value("bold").toString() == "true" ? QFont::Bold : QFont::Normal);
                    }
                    else if (name == "DoxygenTags") {
                        doxygenTagsFormat.setForeground(QColor(xmlReader.attributes().value("foreground").toString()));
                    }
                }
            }
            colorSchemeFile.close();
        }

        void initRegexp() {
            singleLineCommentExp.setPattern(RTC_IDE_REGEXP_SINGLE_LINE_COMMENT);
            commentStartExp.setPattern(RTC_IDE_REGEXP_MULTI_LINE_COMMENT_START);
            commentEndExp.setPattern(RTC_IDE_REGEXP_MULTI_LINE_COMMENT_END);
            preprocessExp.setPattern(RTC_IDE_REGEXP_PREPROCESSOR);
            numberDecExp.setPattern(RTC_IDE_REGEXP_NUMBER_DECIMAL);
            numberHexExp.setPattern(RTC_IDE_REGEXP_NUMBER_HEX);
            numberOctExp.setPattern(RTC_IDE_REGEXP_NUMBER_OCTAL);
            numberBinExp.setPattern(RTC_IDE_REGEXP_NUMBER_BINARY);
            numberFloatExp.setPattern(RTC_IDE_REGEXP_NUMBER_FLOAT);
            stringApostropheExp.setPattern(RTC_IDE_REGEXP_STRING_APOSTROPHE);
            stringQuotationExp.setPattern(RTC_IDE_REGEXP_STRING_QUOTATION);
            stringRawLiteralExp.setPattern(RTC_IDE_REGEXP_STRING_RAW_LITERAL);
            identifierExp.setPattern(RTC_IDE_REGEXP_IDENTIFIER);
        }
    protected:
        void highlightBlock(const QString &text) override {
            for (const HighlightingRule &rule: highlightingRules) {
                QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
                while (matchIterator.hasNext()) {
                    QRegularExpressionMatch match = matchIterator.next();
                    setFormat(static_cast<int>(match.capturedStart()), static_cast<int>(match.capturedLength()), rule.format);
                }
            }

            QRegularExpressionMatch preprocessorMatch = preprocessExp.match(text);
            if (preprocessorMatch.hasMatch()) {
                setFormat(static_cast<int>(preprocessorMatch.capturedStart()), static_cast<int>(preprocessorMatch.capturedEnd()), preprocessorFormat);
            }

            QRegularExpressionMatch singleLineCommentMatch = singleLineCommentExp.match(text);
            if (singleLineCommentMatch.hasMatch()) {
                setFormat(static_cast<int>(singleLineCommentMatch.capturedStart()), static_cast<int>(singleLineCommentMatch.capturedLength()),
                          commentFormat);
            }
            setCurrentBlockState(0);

            int startIndex = 0;
            if (previousBlockState() != 1) {
                startIndex = static_cast<int>(text.indexOf(commentStartExp));
            }

            while (startIndex >= 0) {
                QRegularExpressionMatch match = commentEndExp.match(text, startIndex);
                int endIndex = static_cast<int>(match.capturedStart());
                int commentLength;

                if (endIndex == -1) {
                    setCurrentBlockState(1);
                    commentLength = static_cast<int>(text.length() - startIndex);
                } else {
                    commentLength = static_cast<int>(endIndex - startIndex + match.capturedLength());
                }

                setFormat(startIndex, commentLength, commentFormat);
                startIndex = static_cast<int>(text.indexOf(commentStartExp, startIndex + commentLength));
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
        QRegularExpression numberDecExp, numberHexExp, numberOctExp, numberBinExp, numberFloatExp;
        QRegularExpression stringApostropheExp, stringQuotationExp, stringRawLiteralExp, identifierExp;

        QTextCharFormat keywordFormat, lineNumberFormat,
                        searchResultFormat, parentheseFormat,
                        bracketFormat, braceFormat,
                        currentLineFormat, currentLineNumberFormat,
                        OccurrencesFormat, UnusedOccurrencesFormat,
                        NumberFormat, stringFormat, typeFormat,
                        virtualMethodFormat, functionFormat,
                        primitiveTypeFormat, operatorFormat,
                        preprocessorFormat, identifierFormat,
                        commentFormat, DoxygenCommentTagFormat,
                        propertyFormat, warningFormat,
                        errorFormat, errorContextFormat,
                        declarationFormat, selectionFormat,
                        quotationFormat, apostropheFormat, doxygenTagsFormat;
    };
//}

#endif //RetoCodeEditor_CFamilyHighlighter_h