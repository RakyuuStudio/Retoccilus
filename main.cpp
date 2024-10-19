#include "Source/Engine3D/SceneWidget/SceneWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Retoccilus::Engine3D::Scene::RtSceneWidget sw;
    sw.show();
    return app.exec();
}