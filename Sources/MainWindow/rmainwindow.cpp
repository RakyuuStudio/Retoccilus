#include "rmainwindow.h"
#include "ui_rmainwindow.h"

#include "RCodeEditor.h"
#include "RCFamilyHighlighter.h"
#include "RSyntaxStyle.h"

RMainWindow::RMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::RMainWindow) {
    ui->setupUi(this);
//    auto *rce = new RCodeEditor(this);
//    rce->setStyleSheet("QTextEdit { background-color: #282a36; color: #ffffff; font-family: \"Consolas\" }");
//    loadStyle(":/config/Configuration/RCodeEditor/ColorThemes/Dracula.xml");
//    rce->setHighlighter();
//    setCentralWidget(rce);

    m_styles = {
            {"Default", RSyntaxStyle::defaultStyle()}
    };

    m_highlighters = {
            {"None", nullptr},
            {"C++",  new RCFamilyHighlighter},
    };

    m_codeEditor->setSyntaxStyle(m_styles[0].second);
//    m_codeEditor->setCompleter  (m_completers[0].second);
    m_codeEditor->setHighlighter(m_highlighters[0].second);
}

RMainWindow::~RMainWindow() {
    delete ui;
}

void RMainWindow::loadStyle(QString path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;
    auto style = new RSyntaxStyle(this);
    if (!style->load(file.readAll())) {
        delete style;
        return;
    }

    m_styles.append({style->name(), style});
}