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