#ifndef RETOCCILUS_RCODEEDITORHANDLER_H
#define RETOCCILUS_RCODEEDITORHANDLER_H

#include <QMainWindow>

#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RCodeEditorHandler; }
QT_END_NAMESPACE

class RCodeEditorHandler : public QMainWindow {
Q_OBJECT

public:
    explicit RCodeEditorHandler(QWidget *parent = nullptr);

    ~RCodeEditorHandler() override;
public:
    void performConnection();
    void initUIs();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    //Components
    QWidget *centralwidget;
    QWidget *mainWidget, *titleBarWidget;
    QHBoxLayout *titleBarLayout;

private:
    QPointF last;
    Ui::RCodeEditorHandler *ui;
};


#endif //RETOCCILUS_RCODEEDITORHANDLER_H
