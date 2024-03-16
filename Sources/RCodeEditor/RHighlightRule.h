//==-- RHighlightRule.h - RCodeEditor Highlighting Rule Header -----------*- C++ -*-==//
//* Based on QCodeEditor Project by Megaxela                                         *//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.                      *//
//* Retoccilus licensed under GNU GPL v3 license.                                    *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.           *//
//* QCodeEditor licensed under MIT License                                           *//
//* See <https://github.com/Megaxela/QCodeEditor>. for license information.          *//
//==-----------------------------------------------------------------------------====*//

#ifndef RetoCodeEditor_HighlightRule_h
#define RetoCodeEditor_HighlightRule_h

#pragma once

#include <QRegularExpression>
#include <QString>

struct QHighlightRule {
    QHighlightRule() :
            pattern(),
            formatName() {}

    QHighlightRule(QRegularExpression p, QString f) :
            pattern(std::move(p)),
            formatName(std::move(f)) {}

    QRegularExpression pattern;
    QString formatName;
};

#endif