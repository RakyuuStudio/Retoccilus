#ifndef RMainWindow_H
#define RMainWindow_H

#include <QMainWindow>

namespace RetoNS {
    QT_BEGIN_NAMESPACE
    namespace Ui {
        class RMainWindow;
    }
    QT_END_NAMESPACE
    class RMainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit RMainWindow(QWidget *parent = nullptr);

        ~RMainWindow() override;

    private:
        Ui::RMainWindow *ui;
    };
} //RetoNS

#endif //RMainWindow_H