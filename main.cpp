#include <QtQml>
#include <QApplication>
#include <RCodeEditor.h>
#include <RCFamilyHighlighter.h>
#include "Sources/RCodeEditor_UI/retocodeeditor_uihandler.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//
//    QQmlApplicationEngine engine;
//    const QUrl codeEditorUIUrl(QStringLiteral("qrc:/uiQmls/UIs/RCodeEditor_UI/RCodeEditorUIHandler.qml"));
//    engine.load(codeEditorUIUrl);

    RetoCodeEditor_UIHandler *codeEditorUI;
    codeEditorUI = new RetoCodeEditor_UIHandler();
    codeEditorUI->show();

//    auto *editor = new RCodeEditor();
//    auto *highlighter = new RCFamilyHighlighter(editor->document());
//    editor->setStyleSheet("background-color: #282a36; color: #FFFFFF; font-family: 'Consolas';");
//    editor->show();

    return QApplication::exec();
}
