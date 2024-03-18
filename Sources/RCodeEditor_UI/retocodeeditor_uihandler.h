#ifndef RetoCodeEditor_UiHandler
#define RetoCodeEditor_UiHandler

#include <QMainWindow>
#include <QWidget>
#include <QTabBar>

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
    void commentBlockSlot();
    void unCommentBlockSlot();
    void commentLineSlot();
    void unCommentLineSlot();
    void openPreference();
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
    QAction *undo, *redo, *cut, *copy, *copyPathRef, *paste, *deleteSelect;
    QAction *find, *findUsage, *replace, *replaceUsage, *findInFiles, *replaceInFiles;
    QAction *switchCase, *upperCase, *lowerCase, *comment, *uncomment, *blockComment, *blockUncomment;
    QAction *indent, *unindent, *autoIndent, *autoUnindent, *autoFormat;
    QAction *goToLine, *goToDefinition, *goToDeclaration, *goToImplementation, *goToUsage, *goToInclude;
    QAction *preference;
    QString SaveFileName;

    RCodeEditor *codeEditor;
    RCFamilyHighlighter *highlighter;
    QTabBar *navigateBar;
    Ui::RetoCodeEditor_UIHandler *ui;
    SARibbonBar* ribbonBar {nullptr};
};


#endif // RetoCodeEditor_UiHandler
