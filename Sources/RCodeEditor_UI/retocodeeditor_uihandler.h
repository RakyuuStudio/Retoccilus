#ifndef RetoCodeEditor_UiHandler
#define RetoCodeEditor_UiHandler

#include <QMainWindow>
#include <QWidget>

//SARibbon
#include <SARibbonBar.h>
#include <SARibbonMainWindow.h>

#include "RCodeEditor/RCodeEditor.h"
#include "RCodeEditor/RCFamilyHighlighter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RetoCodeEditor_UIHandler; }
QT_END_NAMESPACE

class RetoCodeEditor_UIHandler : public SARibbonMainWindow {
    Q_OBJECT

public:
    explicit RetoCodeEditor_UIHandler(QWidget *parent = nullptr);
    ~RetoCodeEditor_UIHandler() override;

private slots:
    void insertPageSlot();
    void saveSlot();
private:
    void init();
    void performConnection();
    void insertPage();
    void save();
    void judgeSourceType();
    //SARibbon
    void initCategoryPages();
private:
    QAction *actSave, *actNew, *actOpen, *actSaveAs, *actReloadCMake, *actExport, *actPrint, *actExit;
    QString SaveFileName;
    Ui::RetoCodeEditor_UIHandler *ui;
    SARibbonBar* ribbonBar {nullptr};
};


#endif // RetoCodeEditor_UiHandler
