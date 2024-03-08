//
// Created by ryan_ on 2024/3/8.
//

#ifndef RETOCCILUS_RCE_UI_H
#define RETOCCILUS_RCE_UI_H

#include <QMainWindow>
#include <QPushButton>

//using namespace RFluentUI;

QT_BEGIN_NAMESPACE
namespace Ui { class RCE_UI; }
QT_END_NAMESPACE

class RCE_UI : public QMainWindow {
Q_OBJECT

public:
    explicit RCE_UI(QWidget *parent = nullptr);

    ~RCE_UI() override;

public:
    void initUI();

public slots:
    void onCloseTriggered();
    void onMinimizeTriggered();
    void onMaximizeTriggered();

private:
    QPushButton *closeButton;
    FluentPushButton *minimizeButton;
    FluentPushButton *maximizeButton;
private:
    Ui::RCE_UI *ui;
};


#endif //RETOCCILUS_RCE_UI_H
