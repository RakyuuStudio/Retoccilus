//==-- RStyleSyntaxHighlighter.h - RCodeEditor SyntaxStyle Header --------*- C++ -*-==//
//* Based on QCodeEditor Project by Megaxela                                         *//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.                      *//
//* Retoccilus licensed under GNU GPL v3 license.                                    *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.           *//
//* QCodeEditor licensed under MIT License                                           *//
//* See <https://github.com/Megaxela/QCodeEditor>. for license information.          *//
//==-----------------------------------------------------------------------------====*//
#pragma once

#include <QSyntaxHighlighter>

class RSyntaxStyle;

class RStyleSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    explicit RStyleSyntaxHighlighter(QTextDocument* document=nullptr) : rSyntaxStyle(nullptr) , QSyntaxHighlighter(document){

    }
    void setSyntaxStyle(RSyntaxStyle* style) {
        rSyntaxStyle = style;
    }

    RSyntaxStyle* syntaxStyle() const {
        return rSyntaxStyle;
    }
private:
    RSyntaxStyle* rSyntaxStyle;
};