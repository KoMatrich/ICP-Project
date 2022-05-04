#pragma once

#include <QMainWindow>

#include "Graph/ERD/View.h"
#include "Graph/SEQ/View.h"
#include "TextCode.h"

/// @brief Main window class
class MainWindow : public QMainWindow
{
public:
    /// @brief              Main window constructor
    /// @param fileName     File to be opened
    MainWindow(const QString& fileName = {});
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    /// @brief      Saves file
    /// @return     True on success, else False
    bool save();
    /// @brief      Saves file as
    /// @return     True on success, else False
    bool saveAs();
    /// @brief      Shows about tab
    void about();
    /// @brief      Initializes window
    void init();
    /// @brief      Creates action menu
    void createActions();
    /// @brief      Creates status bar
    void createStatusBar();
    /// @brief      Get settings
    void readSettings();
    /// @brief      Saves settings
    void writeSettings();
    /// @brief      Saves modified file
    /// @return     True on success, else False
    bool maybeSave();
    /// @brief              Loads file to editor
    /// @param fileName     File to be loaded
    void loadFile(const QString& fileName);
    /// @brief              Saves editor file
    /// @param fileName     Name of save
    bool saveFile(const QString& fileName);
    /// @brief              Updates current file
    /// @param fileName     File name
    void setCurrentFile(const QString& fileName);
    /// @brief              Returns
    /// @param fullFileName File name
    /// @return             Clean file path
    QString strippedName(const QString& fullFileName);

private:
    MainTextEdit* mainTextEdit;
    DebugTextEdit* debugTextEdit;

    ERDView* erdView;
    SEQView* seqView;

    /// @brief Name of current file
    QString curFile;
    /// @brief Indicates if files has been modified
    bool isUntitled;
};
