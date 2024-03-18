//
// Created by ryan_ on 2024/3/18.
//

#ifndef RETOCCILUS_RCODEEDITORPREFERENCEWINDOW_H
#define RETOCCILUS_RCODEEDITORPREFERENCEWINDOW_H

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class RCodeEditorPreferenceWindow; }
QT_END_NAMESPACE

class RCodeEditorPreferenceWindow : public QWidget {
Q_OBJECT

public:
    explicit RCodeEditorPreferenceWindow(QWidget *parent = nullptr);

    ~RCodeEditorPreferenceWindow() override;
public slots:
    void changeIndex(QTreeWidgetItem *item, int col);
private:
    Ui::RCodeEditorPreferenceWindow *ui;
};


#endif //RETOCCILUS_RCODEEDITORPREFERENCEWINDOW_H
