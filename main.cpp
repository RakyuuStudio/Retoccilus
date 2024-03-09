#include <QtQml>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl codeEditorUIUrl(QStringLiteral("qrc:/uiQmls/UIs/RCodeEditor_UI/RCodeEditorUIHandler.qml"));
    engine.load(codeEditorUIUrl);

    return QApplication::exec();
}
