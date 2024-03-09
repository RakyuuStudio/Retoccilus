//
// Created by ryan_ on 2024/3/8.
//

#ifndef RETOCCILUS_RCODEEDITORHANDLER_H
#define RETOCCILUS_RCODEEDITORHANDLER_H

#include <QMainWindow>


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
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QPointF last;
    Ui::RCodeEditorHandler *ui;
};


#endif //RETOCCILUS_RCODEEDITORHANDLER_H
