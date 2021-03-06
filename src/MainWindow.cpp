#include "MainWindow.h"

MainWindow::MainWindow(const QString& fileName)
{
    init();

    Semantics& sem = Semantics::getInstance();

    sem.setERDView(erdView);
    sem.setSEQView(seqView);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == mainTextEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Z && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            HistoryService::restoreHistorySnapshot();
            return true;
        } else {
            HistoryService::takeHistorySnapshot(); //before change
            return false;
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();

    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this,
                       QStringLiteral("About ICP UML Project"),
                       QStringLiteral("<b>Authors:</b> <i>Vitezslav Kriz & Martin Kocich</i>"));
}

void MainWindow::init()
{
    isUntitled = true;

    /*  window layout
     *  #======================#
     *  #mainTextEdit  |       #
     *  #==============|tabs   #
     *  #debugTextEdit |       #
     *  #======================#
    */

    //left horizontal split
    QSplitter* hsplit = new QSplitter(Qt::Horizontal, this);

    //center vertical split
    QSplitter* vsplit = new QSplitter(Qt::Vertical, hsplit);
    hsplit->addWidget(vsplit);

    //left bottom
    debugTextEdit = new DebugTextEdit(vsplit);
    DebugService::setEndpoint(debugTextEdit);

    //left top
    mainTextEdit = new MainTextEdit(vsplit);
    mainTextEdit->installEventFilter(this);
    CodeService::setEndpoint(mainTextEdit);
    HistoryService::setEndpoint(mainTextEdit);

    vsplit->addWidget(mainTextEdit);

    vsplit->addWidget(debugTextEdit);

    //set height proportion
    QList<int> v_list = vsplit->sizes();
    v_list.replace(0, this->height() / 0.2);
    v_list.replace(1, this->height() / 0.8);
    vsplit->setSizes(v_list);

    //right
    QTabWidget* tabs = new QTabWidget(hsplit);
    tabs->setTabPosition(QTabWidget::East);
    hsplit->addWidget(tabs);

    //right first
    erdView = new ERDView(tabs);
    tabs->addTab(erdView, "Class");

    //right second
    seqView = new SEQView(tabs);
    tabs->addTab(seqView, "Sequence");

    //set width proportion
    QList<int> h_list = hsplit->sizes();
    h_list.replace(0, this->width() / 0.5);
    h_list.replace(1, this->width() / 0.5);
    hsplit->setSizes(h_list);

    setCentralWidget(hsplit);

    createActions();
    createStatusBar();

    readSettings();

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createActions()
{
    auto* fileMenu = menuBar()->addMenu(QStringLiteral("File"));
    fileMenu->addAction(QStringLiteral("New"),
                        [this]() {loadFile(""); },
                        QKeySequence::New);
    fileMenu->addAction(QStringLiteral("Open"),
                        [this]() {loadFile(QFileDialog::getOpenFileName(this)); },
                        QKeySequence::Open);
    fileMenu->addAction(QStringLiteral("Save"),
                        [this]() {save(); },
                        QKeySequence::Save);
    fileMenu->addAction(QStringLiteral("Save As"),
                        [this]() {saveAs(); },
                        QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(QStringLiteral("Close"),
                        [this]() {close(); },
                        QKeySequence::SaveAs);

    auto* editMenu = menuBar()->addMenu(QStringLiteral("Edit"));
    auto* undoAct = editMenu->addAction(QStringLiteral("Undo"),
                                        [this]() {HistoryService::restoreHistorySnapshot(); },
                                        QKeySequence::Undo);
    auto* cutAct = editMenu->addAction(QStringLiteral("Cut"),
                                       [this]() {mainTextEdit->cut(); },
                                       QKeySequence::Cut);
    auto* copyAct = editMenu->addAction(QStringLiteral("Copy"),
                                        [this]() {mainTextEdit->copy(); },
                                        QKeySequence::Copy);
    auto* pasteAct = editMenu->addAction(QStringLiteral("Paste"),
                                         [this]() {mainTextEdit->paste(); },
                                         QKeySequence::Paste);

    auto* helpMenu = menuBar()->addMenu(QStringLiteral("Help"));
    helpMenu->addAction(QStringLiteral("About"), [this]() {about(); });

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);

    auto* editToolBar = addToolBar(QStringLiteral("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

    auto* fileToolBar = addToolBar(QStringLiteral("File"));
    fileToolBar->addAction(QStringLiteral("Syntax refresh"),
                           [this]() {mainTextEdit->syntax_reload(); });
    fileToolBar->addAction(QStringLiteral("Undo"),
                           [this]() {HistoryService::restoreHistorySnapshot(); });

    connect(mainTextEdit->document(), &QTextDocument::contentsChanged,
            [this]() { setWindowModified(true); });

    connect(mainTextEdit, &MainTextEdit::copyAvailable,
            [cutAct](bool s) {cutAct->setEnabled(s); });
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
    QString fileName = settings.value("last_file", QString("")).toString();
    move(pos);
    resize(size);
    loadFile(fileName);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("last_file", curFile);
}

bool MainWindow::maybeSave()
{
    if (!mainTextEdit->document()->isModified())
        return true;

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
    else //(ret == QMessageBoX::Discard)
        return true;
}

void MainWindow::loadFile(const QString& fileName)
{
    if (!maybeSave()) {
        //action canceled
        return;
    }

    //disable code change detection
    HighlightService::setEnabled(false);
    //clear all background formating
    CodeService::clearBackground();
    //clear semantics classes
    Semantics::getInstance().removeClasses();
    //clear semantics classes
    Semantics::getInstance().removeSequences();
    //clear text editor
    mainTextEdit->clear();

    erdView->update();
    seqView->update();

    setCurrentFile(fileName);

    VitaClear();

    if (fileName.isEmpty()) {
        VitaPrint("Write some code or open a document to get started.");
        HighlightService::setEnabled(true);
        return;
    }

    VitaPrint("New file loaded");


    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        HighlightService::setEnabled(true);
        mainTextEdit->syntax_reload();
        save();
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainTextEdit->setPlainText(in.readAll());
    HighlightService::setEnabled(true);
    mainTextEdit->syntax_reload();


    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("File loaded"), 2000);
    save();
}

bool MainWindow::saveFile(const QString& fileName)
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

void MainWindow::setCurrentFile(const QString& fileName)
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

QString MainWindow::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
