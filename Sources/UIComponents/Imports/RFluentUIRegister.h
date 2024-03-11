#ifndef RetoUI_FluentUI_Register_h
#define RetoUI_FluentUI_Register_h

#include <QObject>
#include <QQuickWindow>
#include <QJsonObject>
#include "RFluentUIAffactor.h"
#include "RFluentUIApplication.h"

class RFluentUIRegister : public QObject {
Q_OBJECT

Q_PROPERTY_AUTOMATICALLY(QQuickWindow*, from)
Q_PROPERTY_AUTOMATICALLY(QQuickWindow*, to)
Q_PROPERTY_AUTOMATICALLY(QString, path)
public:
    explicit RFluentUIRegister(QObject *parent = nullptr) : QObject(parent) {
        from(nullptr);
        to(nullptr);
        path("");
    }

    Q_INVOKABLE void launch(const QJsonObject &arg = {}) {
        RFluentApplication::getInstance()->navigate(path(), arg, this);
    }

    Q_INVOKABLE void onResult(const QJsonObject &data = {}) {
        emit result(data);
    }

    Q_SIGNAL void result(const QJsonObject &data);
}

#endif