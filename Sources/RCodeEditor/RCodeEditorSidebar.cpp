#include "RCodeEditorSidebar.h"

QSize SidebarArea::sizeHint() const {
    return QSize(codeEditor->sideBarAreaWidth(), 0);
}

SidebarArea::SidebarArea(RCodeEditor *editor) : QWidget(editor), codeEditor(editor) {

}

void SidebarArea::paintEvent(QPaintEvent *event) {
    codeEditor->sideBarAreaPaintEvent(event);
}

