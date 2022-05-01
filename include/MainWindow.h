#pragma once

#include <QMainWindow>

#include "Graph/ERDview.h"
#include "Graph/SEQview.h"
#include "TextCode.h"
#include "TextDebug.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(const QString& fileName = {});

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    bool save();
    bool saveAs();
    void about();

private:
    void init();

    void createActions();
    void createStatusBar();

    void readSettings();
    void writeSettings();
    bool maybeSave();

    void loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);

    void setCurrentFile(const QString& fileName);

    QString strippedName(const QString& fullFileName);

private:
    MainTextEdit* mainTextEdit;
    DebugTextEdit* debugTextEdit;

    ERDView* erdView;
    SEQView* seqView;

    QString curFile;
    bool isUntitled;
};
