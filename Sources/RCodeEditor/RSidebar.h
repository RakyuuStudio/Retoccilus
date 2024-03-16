//==-- RSideBar.h - RCodeEditor Left Sidebar Interface Header - ----------*- C++ -*-==//
//* Copyright (c) 2023-2024 Rakyuu Studio. All rights reserved.                      *//
//* Retoccilus licensed under GNU GPL v3 license.                                    *//
//* See https://www.gnu.org/licenses/gpl-3.0.html for license information.           *//
//==-----------------------------------------------------------------------------====*//
#ifndef RCodeEditor_Sidebar_h
#define RCodeEditor_Sidebar_h

#pragma once

#include "RSyntaxStyle.h"
#include "RCodeEditor.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QTextEdit>
#include <QFontMetrics>

class RCodeEditor;

class RSideBar : public QWidget
{
public:
    explicit RSideBar(RCodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {}

    [[nodiscard]] QSize sizeHint() const override
    {
        return QSize(codeEditor->sidebarWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->sidebarPaintEvent(event);
    }

private:
    RCodeEditor *codeEditor;
};

#endif