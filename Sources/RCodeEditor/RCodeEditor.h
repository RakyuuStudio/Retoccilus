#ifndef RCodeEditor_H
#define RCodeEditor_H

#include <QPlainTextEdit>
#include <QTextCursor>
#include <QMimeData>
#include <QFile>

#include "RXmlReader.h"
#include "RetoCodeEditor.h"
#include "RXmlReader.h"

#include "RCodeEditorSidebar.h"
#include "RSyntaxHighlighter.h"

using namespace RetoUtils;

class RCodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit RCodeEditor(QWidget *widget = nullptr); //Constructor
    ~RCodeEditor(); //Destructor
    int getFirstVisibleBlock();

    void sideBarAreaPaintEvent(QPaintEvent *event);
    int sideBarAreaWidth();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void updateSideBarAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateSideBarArea(const QRect &rect, int dy);

private:
    QWidget *sideBarArea;

    QStringList keywordList;
    XmlReader xr;
};

#endif