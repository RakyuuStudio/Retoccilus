#ifndef RetoUI_FluentUI_Main_h
#define RetoUI_FluentUI_Main_h

#include <QObject>
#include <QQmlEngine>
#include "SingletonHandle.h"

//Components

class RFluentUI_Main : public QObject {
Q_OBJECT

    SINGLETON_HANDLE(RFluentUI_Main)

public:
    Q_DECL_EXPORT void registerType(QQmlEngine *engine);

    void registerTypes(const char *url);

    void initEngine(QQmlEngine *engine, const char *url);

private:
    int major = 1, minor = 0;
    const char *url = "RetoUI.FluentUI";
};

#endif // RetoUI_FluentUI_Main_h