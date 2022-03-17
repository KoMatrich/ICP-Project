#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    init();
    setCurrentFile("");
}

MainWindow::MainWindow(const QString &fileName)
{
    init();
    loadFile(fileName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();

    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    MainWindow *other = new MainWindow;
    other->move(x() + 40, y() + 40);
    other->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        MainWindow *existing = findMainWindow(fileName);
        if (existing) {
            existing->show();
            existing->raise();
            existing->activateWindow();
            return;
        }

        if (isUntitled && mainTextEdit->document()->isEmpty()
                && !isWindowModified()) {
            loadFile(fileName);
        } else {
            MainWindow *other = new MainWindow(fileName);
            if (other->isUntitled) {
                delete other;
                return;
            }
            other->move(x() + 40, y() + 40);
            other->show();
        }
    }
}

bool MainWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

///@TODO
void MainWindow::about()
{
   QMessageBox::about(this, tr("About SDI"),
            tr("<b>@TODO</b>"));
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
}

void MainWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);

    isUntitled = true;

    QSplitter *hsplit = new QSplitter(Qt::Horizontal);

    //left
    QSplitter *vsplit = new QSplitter(Qt::Vertical, hsplit);
    hsplit->addWidget(vsplit);

    //right
    QTabWidget *tabs = new QTabWidget(hsplit);
    tabs->setTabPosition(QTabWidget::East);
    hsplit->addWidget(tabs);

    //left top
    mainTextEdit = new MainTextEdit(vsplit);
    mainTextEdit->acceptRichText();
    vsplit->addWidget(mainTextEdit);

    //left bottom
    debugTextEdit = new DebugTextEdit(vsplit);
    vsplit->addWidget(mainTextEdit);

    //right first
    erdView = new ERDview(mainTextEdit);
    tabs->addTab(erdView, "Class");

    //right second
    seqView = new SEQview(mainTextEdit);
    tabs->addTab(seqView, "Sequence");

    //??
    syntax = new Highlighter(mainTextEdit->document());
    setCentralWidget(hsplit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(mainTextEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Close this window"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), mainTextEdit, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), mainTextEdit, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), mainTextEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(mainTextEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(mainTextEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(1280, 720)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::maybeSave()
{
    if (mainTextEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("SDI"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainTextEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << mainTextEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = tr("document%1.txt").arg(sequenceNumber++);
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
    }

    mainTextEdit->document()->setModified(false);
    setWindowModified(false);
    setWindowFilePath(curFile);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

MainWindow *MainWindow::findMainWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QWidget *widget, qApp->topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin && mainWin->curFile == canonicalFilePath)
            return mainWin;
    }
    return 0;
}
