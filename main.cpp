#include <QtQml>
#include <QApplication>
#include <RCodeEditor.h>
#include <RCFamilyHighlighter.h>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//
//    QQmlApplicationEngine engine;
//    const QUrl codeEditorUIUrl(QStringLiteral("qrc:/uiQmls/UIs/RCodeEditor_UI/RCodeEditorUIHandler.qml"));
//    engine.load(codeEditorUIUrl);

RCodeEditor *rce = new RCodeEditor();
rce->setStyleSheet("background-color: #2E2E2E; color: #FFFFFF;");
rce->show();
RCFamilyHighlighter *highlighter = new RCFamilyHighlighter(rce->document());
    return QApplication::exec();
}
