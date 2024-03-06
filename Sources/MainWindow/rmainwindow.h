#ifndef RMainWindow_H
#define RMainWindow_H

#include <QMainWindow>
#include <RSyntaxStyle.h>
#include <RStyleSyntaxHighlighter.h>
#include <QCompleter>
#include "RCodeEditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RMainWindow; }
QT_END_NAMESPACE

class RMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit RMainWindow(QWidget *parent = nullptr);

    ~RMainWindow() override;

private:
    Ui::RMainWindow *ui;


    void loadStyle(QString path);

    RCodeEditor *m_codeEditor;
    QVector<QPair<QString, QString>> m_codeSamples;
    QVector<QPair<QString, QCompleter*>> m_completers;
    QVector<QPair<QString, RStyleSyntaxHighlighter*>> m_highlighters;
    QVector<QPair<QString, RSyntaxStyle*>> m_styles;
};

#endif //RMainWindow_H