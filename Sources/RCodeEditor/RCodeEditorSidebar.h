#ifndef RCodeEditorSidebar_h
#define RCodeEditorSidebar_h

#include <QWidget>

class RCodeEditor;

class SidebarArea : public QWidget {
public:
    SidebarArea(RCodeEditor *editor);
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    RCodeEditor *codeEditor;
};

#include "RCodeEditor.h"

#endif //RCodeEditorSidebar_h