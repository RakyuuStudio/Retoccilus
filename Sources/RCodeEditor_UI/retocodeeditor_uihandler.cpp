#include "retocodeeditor_uihandler.h"
#include "ui_RetoCodeEditor_UIHandler.h"

#include "SARibbonMainWindow.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include "SARibbonCheckBox.h"
#include "SARibbonColorToolButton.h"
#include "SARibbonComboBox.h"
#include "SARibbonCustomizeDialog.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonGallery.h"
#include "SARibbonLineEdit.h"
#include "SARibbonMenu.h"
#include "SARibbonPannel.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonToolButton.h"

#include <QtWidgets>
#include <QFile>
#include <QFileDialog>
#include "RCodeEditor/PreferenceHandle/rcodeeditorpreferencewindow.h"

RetoCodeEditor_UIHandler::RetoCodeEditor_UIHandler(QWidget *parent) :
        SARibbonMainWindow(parent), ui(new Ui::RetoCodeEditor_UIHandler)
{
    ui->setupUi(this);
//    setWindowTitle("RCodeEditor");
    this->setWindowState(Qt::WindowMaximized);
    init();
    performConnection();
}

RetoCodeEditor_UIHandler::~RetoCodeEditor_UIHandler() {
    delete ui;
}

void RetoCodeEditor_UIHandler::performConnection() {
    connect(actNew, &QAction::triggered, this, &RetoCodeEditor_UIHandler::insertPageSlot);
    connect(actSave, &QAction::triggered, this, &RetoCodeEditor_UIHandler::saveSlot);
    connect(comment, &QAction::triggered, this, &RetoCodeEditor_UIHandler::commentLineSlot);
    connect(uncomment, &QAction::triggered, this, &RetoCodeEditor_UIHandler::unCommentLineSlot);
    connect(preference, &QAction::triggered, this, &RetoCodeEditor_UIHandler::openPreference);
}

void RetoCodeEditor_UIHandler::saveSlot() {
    save();
}

void RetoCodeEditor_UIHandler::save() {
//    auto *editor = qobject_cast<RCodeEditor *>(ui->tabWidget->currentWidget());
//    if (editor) {
//        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("All Files (*.*)"));
//        if (fileName.isEmpty()) {
//            return;
//        }
//        QFile file(fileName);
//        if (!file.open(QIODevice::WriteOnly)) {
//            ui->statusbar->showMessage(tr("Unable to open file"));
//            return;
//        }
//        QTextStream out(&file);
//        out << editor->toPlainText();
//        file.close();
//        ui->statusbar->showMessage(tr("File saved"), 2000);
//        SaveFileName = fileName;
//        judgeSourceType();
//    }
}

void RetoCodeEditor_UIHandler::judgeSourceType() {
//    QString extensionType = SaveFileName.split('.').last();
//    static const QMap<QString, QString> extensionToIconMap = {
//            { "c", ":/icons/Icons/Icon_CSource.png" },
//            { "cpp", ":/icons/Icons/Icon_CppSource.png" },
//            { "h", ":/icons/Icons/Icon_CHeader.png" },
//            { "hpp", ":/icons/Icons/Icon_CppHeader.png" },
//            { "css", ":/icons/Icons/Icon_Css.png" },
//            { "html", ":/icons/Icons/Icon_HTML.png" },
//            { "htm", ":/icons/Icons/Icon_HTML.png" },
//            { "js", ":/icons/Icons/Icon_Javascript.png" },
//            { "ts", ":/icons/Icons/Icon_TypeScript.png" },
//            { "json", ":/icons/Icons/Icon_Json.png" },
//            { "xml", ":/icons/Icons/Icon_XML.png" },
//            { "py", ":/icons/Icons/Icon_Python.png" },
//            { "vue", ":/icons/Icons/Icon_Vue.png" },
//            { "jsx", ":/icons/Icons/Icon_React.png" }
//    };
//    if (extensionToIconMap.contains(extensionType)) {
//        ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(), QIcon(extensionToIconMap[extensionType]));
//    }
//    else if (SaveFileName.contains("CMakeLists.txt") || SaveFileName.contains("CMakeCache.txt")) {
//        ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(), QIcon(":/icons/Icons/Icon_CMake.png"));
//    }
}

void RetoCodeEditor_UIHandler::insertPage() {
}

void RetoCodeEditor_UIHandler::insertPageSlot() {
    insertPage();
}

void RetoCodeEditor_UIHandler::commentLineSlot() {
    codeEditor->commentLine();
}

void RetoCodeEditor_UIHandler::unCommentLineSlot() {
    codeEditor->uncommentLine();
}

void RetoCodeEditor_UIHandler::commentBlockSlot() {
    codeEditor->commentBlock();
}

void RetoCodeEditor_UIHandler::unCommentBlockSlot() {
    codeEditor->uncommentBlock();
}

void RetoCodeEditor_UIHandler::initCategoryPages() {
    //File Category
    SARibbonCategory *fileCategory = ribbonBar->addCategoryPage(tr("File"));
    SARibbonCategory *editCategory = ribbonBar->addCategoryPage(tr("Edit"));
    SARibbonCategory *viewCategory = ribbonBar->addCategoryPage(tr("View"));
    SARibbonCategory *reBuildCategory = ribbonBar->addCategoryPage(tr("Rebuild"));
    SARibbonCategory *toolCategory = ribbonBar->addCategoryPage(tr("Tool"));
    SARibbonCategory *VCSCategory = ribbonBar->addCategoryPage(tr("VCS"));
    SARibbonCategory *helpCategory = ribbonBar->addCategoryPage(tr("Help"));

    SARibbonPannel *pannelFile = fileCategory->addPannel(tr("File Control"));
    SARibbonPannel *pannelBuildSystem = fileCategory->addPannel(tr("Build System"));

    //File===================================
    //?
    actSave = new QAction(tr("Save"), this);
    actSave->setToolTip(tr("Save the current file"));
    actSave->setText(tr("Save"));
    actSave->setObjectName("actSave");
    actSave->setIcon(QIcon(":/icons/Icons/Icon_Save.png"));
    actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));

    actNew = new QAction(tr("New"), this);
    actNew->setToolTip(tr("Create a new file"));
    actNew->setText(tr("New"));
    actNew->setObjectName("actNew");
    actNew->setIcon(QIcon(":/icons/Icons/Icon_CreateNew.png"));
    actNew->setShortcut(QKeySequence(QLatin1String("Ctrl+N")));

    actOpen = new QAction(tr("Open"), this);
    actOpen->setToolTip(tr("Open a file"));
    actOpen->setText(tr("Open"));
    actOpen->setObjectName("actOpen");
    actOpen->setIcon(QIcon(":/icons/Icons/Icon_Open.png"));
    actOpen->setShortcut(QKeySequence(QLatin1String("Ctrl+O")));

    actSaveAs = new QAction(tr("Save As"), this);
    actSaveAs->setToolTip(tr("Save the current file as"));
    actSaveAs->setText(tr("Save As"));
    actSaveAs->setObjectName("actSaveAs");
    actSaveAs->setIcon(QIcon(":/icons/Icons/Icon_SaveAs.png"));
    actSaveAs->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+S")));

    actExport = new QAction(tr("Export"), this);
    actExport->setToolTip(tr("Export the current file"));
    actExport->setText(tr("Export"));
    actExport->setObjectName("actExport");
    actExport->setIcon(QIcon(":/icons/Icons/Icon_Export.png"));
    actExport->setShortcut(QKeySequence(QLatin1String("Ctrl+E")));

    actPrint = new QAction(tr("Print"), this);
    actPrint->setToolTip(tr("Print the current file"));
    actPrint->setText(tr("Print"));
    actPrint->setObjectName("actPrint");
    actPrint->setIcon(QIcon(":/icons/Icons/Icon_Print.png"));
    actPrint->setShortcut(QKeySequence(QLatin1String("Ctrl+P")));

    actExit = new QAction(tr("Exit"), this);
    actExit->setToolTip(tr("Exit the application"));
    actExit->setText(tr("Exit"));
    actExit->setObjectName("actExit");
    actExit->setIcon(QIcon(":/icons/Icons/Icon_Exit.png"));
    actExit->setShortcut(QKeySequence(QLatin1String("Ctrl+Q")));

    actReloadCMake = new QAction(tr("Reload CMake"), this);
    actReloadCMake->setToolTip(tr("Reload the CMake file"));
    actReloadCMake->setText(tr("Reload CMake"));
    actReloadCMake->setObjectName("actReloadCMake");
    actReloadCMake->setIcon(QIcon(":/icons/Icons/Icon_CMake.png"));
    actReloadCMake->setShortcut(QKeySequence(QLatin1String("Ctrl+R")));

    pannelFile->addLargeAction(actNew);
    pannelFile->addLargeAction(actOpen);
    pannelFile->addLargeAction(actSave);
    pannelFile->addLargeAction(actSaveAs);
    pannelFile->addLargeAction(actExport);
    pannelFile->addLargeAction(actPrint);
    pannelFile->addLargeAction(actExit);
    pannelBuildSystem->addLargeAction(actReloadCMake);

    //Edit===================================

    undo = new QAction(tr("Undo"), this);
    undo->setToolTip(tr("Undo the last action"));
    undo->setText(tr("Undo"));
    undo->setObjectName("undo");
    undo->setShortcut(QKeySequence(QLatin1String("Ctrl+Z")));

    redo = new QAction(tr("Redo"), this);
    redo->setToolTip(tr("Redo the last action"));
    redo->setText(tr("Redo"));
    redo->setObjectName("redo");
    redo->setShortcut(QKeySequence(QLatin1String("Ctrl+Y")));

    cut = new QAction(tr("Cut"), this);
    cut->setToolTip(tr("Cut the selected text"));
    cut->setText(tr("Cut"));
    cut->setObjectName("cut");
    cut->setShortcut(QKeySequence(QLatin1String("Ctrl+X")));

    copy = new QAction(tr("Copy"), this);
    copy->setToolTip(tr("Copy the selected text"));
    copy->setText(tr("Copy"));
    copy->setObjectName("copy");
    copy->setShortcut(QKeySequence(QLatin1String("Ctrl+C")));

    copyPathRef = new QAction(tr("Copy Path Reference"), this);
    copyPathRef->setToolTip(tr("Copy the path reference of the selected file"));
    copyPathRef->setText(tr("Copy Path Reference"));
    copyPathRef->setObjectName("copyPathRef");
    copyPathRef->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+C")));

    paste = new QAction(tr("Paste"), this);
    paste->setToolTip(tr("Paste the selected text"));
    paste->setText(tr("Paste"));
    paste->setObjectName("paste");
    paste->setShortcut(QKeySequence(QLatin1String("Ctrl+V")));

    deleteSelect = new QAction(tr("Delete"), this);
    deleteSelect->setToolTip(tr("Delete the selected text"));
    deleteSelect->setText(tr("Delete"));
    deleteSelect->setObjectName("deleteSelect");
    deleteSelect->setShortcut(QKeySequence(QLatin1String("Delete")));

    find = new QAction(tr("Find"), this);
    find->setToolTip(tr("Find the selected text"));
    find->setText(tr("Find"));
    find->setObjectName("find");
    find->setShortcut(QKeySequence(QLatin1String("Ctrl+F")));

    findUsage = new QAction(tr("Find Usage"), this);
    findUsage->setToolTip(tr("Find the usage of the selected text"));
    findUsage->setText(tr("Find Usage"));
    findUsage->setObjectName("findUsage");
    findUsage->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+F")));

    replace = new QAction(tr("Replace"), this);
    replace->setToolTip(tr("Replace the selected text"));
    replace->setText(tr("Replace"));
    replace->setObjectName("replace");
    replace->setShortcut(QKeySequence(QLatin1String("Ctrl+H")));

    replaceUsage = new QAction(tr("Replace Usage"), this);
    replaceUsage->setToolTip(tr("Replace the usage of the selected text"));
    replaceUsage->setText(tr("Replace Usage"));
    replaceUsage->setObjectName("replaceUsage");
    replaceUsage->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+H")));

    findInFiles = new QAction(tr("Find in Files"), this);
    findInFiles->setToolTip(tr("Find the selected text in files"));
    findInFiles->setText(tr("Find in Files"));
    findInFiles->setObjectName("findInFiles");
    findInFiles->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+G")));

    replaceInFiles = new QAction(tr("Replace in Files"), this);
    replaceInFiles->setToolTip(tr("Replace the selected text in files"));
    replaceInFiles->setText(tr("Replace in Files"));
    replaceInFiles->setObjectName("replaceInFiles");
    replaceInFiles->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+R")));

    switchCase = new QAction(tr("Switch Case"), this);
    switchCase->setToolTip(tr("Switch the case of the selected text"));
    switchCase->setText(tr("Switch Case"));
    switchCase->setObjectName("switchCase");
    switchCase->setShortcut(QKeySequence(QLatin1String("Ctrl+U")));

    upperCase = new QAction(tr("Upper Case"), this);
    upperCase->setToolTip(tr("Convert the selected text to upper case"));
    upperCase->setText(tr("Upper Case"));
    upperCase->setObjectName("upperCase");
    upperCase->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+U")));

    lowerCase = new QAction(tr("Lower Case"), this);
    lowerCase->setToolTip(tr("Convert the selected text to lower case"));
    lowerCase->setText(tr("Lower Case"));
    lowerCase->setObjectName("lowerCase");
    lowerCase->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+L")));

    comment = new QAction(tr("Comment"), this);
    comment->setToolTip(tr("Comment the selected text"));
    comment->setText(tr("Comment"));
    comment->setObjectName("comment");
    comment->setShortcut(QKeySequence(QLatin1String("Ctrl+/")));

    uncomment = new QAction(tr("Uncomment"), this);
    uncomment->setToolTip(tr("Uncomment the selected text"));
    uncomment->setText(tr("Uncomment"));
    uncomment->setObjectName("uncomment");
    uncomment->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+/")));

    blockComment = new QAction(tr("Block Comment"), this);
    blockComment->setToolTip(tr("Block comment the selected text"));
    blockComment->setText(tr("Block Comment"));
    blockComment->setObjectName("blockComment");
    blockComment->setShortcut(QKeySequence(QLatin1String("Ctrl+Alt+/")));

    blockUncomment = new QAction(tr("Block Uncomment"), this);
    blockUncomment->setToolTip(tr("Block uncomment the selected text"));
    blockUncomment->setText(tr("Block Uncomment"));
    blockUncomment->setObjectName("blockUncomment");
    blockUncomment->setShortcut(QKeySequence(QLatin1String("Ctrl+Alt+Shift+/")));

    indent = new QAction(tr("Indent"), this);
    indent->setToolTip(tr("Indent the selected text"));
    indent->setText(tr("Indent"));
    indent->setObjectName("indent");
    indent->setShortcut(QKeySequence(QLatin1String("Tab")));

    unindent = new QAction(tr("Unindent"), this);
    unindent->setToolTip(tr("Unindent the selected text"));
    unindent->setText(tr("Unindent"));
    unindent->setObjectName("unindent");
    unindent->setShortcut(QKeySequence(QLatin1String("Shift+Tab")));

    autoIndent = new QAction(tr("Auto Indent"), this);
    autoIndent->setToolTip(tr("Auto indent the selected text"));
    autoIndent->setText(tr("Auto Indent"));
    autoIndent->setObjectName("autoIndent");
    autoIndent->setShortcut(QKeySequence(QLatin1String("Ctrl+I")));

    autoUnindent = new QAction(tr("Auto Unindent"), this);
    autoUnindent->setToolTip(tr("Auto unindent the selected text"));
    autoUnindent->setText(tr("Auto Unindent"));
    autoUnindent->setObjectName("autoUnindent");
    autoUnindent->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+I")));

    autoFormat = new QAction(tr("Auto Format"), this);
    autoFormat->setToolTip(tr("Auto format the selected text"));
    autoFormat->setText(tr("Auto Format"));
    autoFormat->setObjectName("autoFormat");
    autoFormat->setShortcut(QKeySequence(QLatin1String("Ctrl+Alt+F")));

    goToLine = new QAction(tr("Go to Line"), this);
    goToLine->setToolTip(tr("Go to the specified line"));
    goToLine->setText(tr("Go to Line"));
    goToLine->setObjectName("goToLine");
    goToLine->setShortcut(QKeySequence(QLatin1String("Ctrl+G")));

    goToDefinition = new QAction(tr("Go to Definition"), this);
    goToDefinition->setToolTip(tr("Go to the definition of the selected text"));
    goToDefinition->setText(tr("Go to Definition"));
    goToDefinition->setObjectName("goToDefinition");
    goToDefinition->setShortcut(QKeySequence(QLatin1String("F12")));

    goToDeclaration = new QAction(tr("Go to Declaration"), this);
    goToDeclaration->setToolTip(tr("Go to the declaration of the selected text"));
    goToDeclaration->setText(tr("Go to Declaration"));
    goToDeclaration->setObjectName("goToDeclaration");
    goToDeclaration->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+D")));

    goToImplementation = new QAction(tr("Go to Implementation"), this);
    goToImplementation->setToolTip(tr("Go to the implementation of the selected text"));
    goToImplementation->setText(tr("Go to Implementation"));
    goToImplementation->setObjectName("goToImplementation");
    goToImplementation->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+I")));

    goToUsage = new QAction(tr("Go to Usage"), this);
    goToUsage->setToolTip(tr("Go to the usage of the selected text"));
    goToUsage->setText(tr("Go to Usage"));
    goToUsage->setObjectName("goToUsage");
    goToUsage->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+U")));

    goToInclude = new QAction(tr("Go to Include"), this);
    goToInclude->setToolTip(tr("Go to the include of the selected text"));
    goToInclude->setText(tr("Go to Include"));
    goToInclude->setObjectName("goToInclude");
    goToInclude->setShortcut(QKeySequence(QLatin1String("Ctrl+Shift+I")));

    preference = new QAction(tr("Preference"), this);
    preference->setToolTip(tr("Open the preference window"));
    preference->setObjectName("preference");
    preference->setShortcut(QKeySequence(QLatin1String("Ctrl+,")));

    SARibbonPannel *pannelEdit = editCategory->addPannel(tr("Text Edit"));
    auto *btnFindAndReplace = new SARibbonToolButton(this);
    btnFindAndReplace->setObjectName("btnFindAndReplace");
    btnFindAndReplace->setText(tr("Find and Replace"));
    btnFindAndReplace->setButtonType(SARibbonToolButton::LargeButton);
    btnFindAndReplace->setIcon(QIcon(":/icons/Icons/Icon_Find.png"));
    btnFindAndReplace->addAction(find);
    btnFindAndReplace->addAction(findUsage);
    btnFindAndReplace->addAction(findInFiles);
    btnFindAndReplace->addAction(replace);
    btnFindAndReplace->addAction(replaceUsage);
    btnFindAndReplace->addAction(replaceInFiles);
    btnFindAndReplace->setPopupMode(QToolButton::InstantPopup);
    pannelEdit->addLargeWidget(btnFindAndReplace);
    pannelEdit->addLargeAction(comment);
    pannelEdit->addLargeAction(uncomment);
    pannelEdit->addLargeAction(preference);
}

void RetoCodeEditor_UIHandler::openPreference() {
    auto *preferenceWindow = new RCodeEditorPreferenceWindow();
    preferenceWindow->show();
}

void RetoCodeEditor_UIHandler::init() {
    //Create SARibbon
    ribbonBar = new SARibbonBar(this);
//    ribbonBar->setVisible(true);
    ribbonBar->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
    ribbonBar->setContentsMargins(5, 0, 5, 0);
    ribbonBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    ribbonBar->setTitleVisible(true);
    sa_set_ribbon_theme(ribbonBar, SARibbonMainWindow::RibbonTheme::RibbonThemeOffice2021Blue);
    initCategoryPages();
    navigateBar = new QTabBar(this);
    navigateBar->addTab("New Source");
    navigateBar->addTab("New Source");
    navigateBar->addTab("New Source");
    navigateBar->addTab("New Source");
    navigateBar->setVisible(true);
    codeEditor = new RCodeEditor();
    highlighter = new RCFamilyHighlighter(codeEditor->document());
    codeEditor->setStyleSheet("background-color: #282a36; color: #FFFFFF; font-family: 'Consolas';");
//    ui->verticalLayout->insertWidget(0, ribbonBar);
    ui->verticalLayout->insertWidget(0, navigateBar);
    ui->verticalLayout->insertWidget(1, codeEditor);
//    QVBoxLayout *layout = new QVBoxLayout();
//    layout->insertWidget(0, ribbonBar);
//    layout->insertWidget(1, navigateBar);
//    layout->insertWidget(2, codeEditor);
//    setLayout(layout);
}