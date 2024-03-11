#ifndef RetoUI_FluentUI_Lifecycle_h
#define RetoUI_FluentUI_Lifecycle_h

#include <QObject>
#include <QQuickWindow>
#include <QtQml/qqml.h>
#include <QQuickItem>
#include <QWindow>
#include <QJsonObject>

class FluWindowLifecycle : public QObject {
Q_OBJECT

    QML_NAMED_ELEMENT(FluWindowLifecycle)
public:
    explicit FluWindowLifecycle(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void onCompleted(QQuickWindow *window) {
        this->_window = window;
        RFluentApplication::getInstance()->addWindow(this->_window);
    }

    Q_INVOKABLE void onDestruction() {

    }

    Q_INVOKABLE void onVisible(bool visible);

    Q_INVOKABLE void onDestroyOnClose() {
        if (_window) {
            RFluentApplication::getInstance()->removeWindow(this->_window);
            _window = nullptr;
        }
    }

private:
    QQuickWindow *_window = nullptr;
};

#endif // RetoUI_FluentUI_Lifecycle_h