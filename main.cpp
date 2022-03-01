#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ICP-Project");
    app.setOrganizationName("QtProject");

    MainWindow *mainWin = new MainWindow;
    mainWin->show();
    return app.exec();
}
