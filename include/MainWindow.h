#pragma once

#include <QMainWindow>

#include "Graph/ERDview.h"
#include "Graph/SEQview.h"
#include "TextCode.h"
#include "TextDebug.h"


class MainWindow : public QMainWindow
{
public:
    MainWindow();
    MainWindow(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

private:
    void init();

    void createActions();
    void createStatusBar();

    void readSettings();
    void writeSettings();
    bool maybeSave();

    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

    void setCurrentFile(const QString &fileName);

    QString strippedName(const QString &fullFileName);
    MainWindow *findMainWindow(const QString &fileName);

    MainTextEdit *mainTextEdit;
    DebugTextEdit *debugTextEdit;
    ERDview *erdView;
    SEQview *seqView;

    QString curFile;
    bool isUntitled;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *syntaxReloadAct;
};
