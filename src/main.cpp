#include <QApplication>
#include "MainWindow.h"
#include "DebugService.h"

Semantics* Semantics::instance = nullptr;
DebugService* DebugService::instance = nullptr;

class App
{
public:
    App(int& argc, char** argv)
        :app(argc, argv)
    {
        app.setApplicationName("ICP-Project");
        app.setOrganizationName("BUT FIT");
    }
    int start()
    {
        mainWin.show();
        return app.exec();
    }
private:
    QApplication app;
    MainWindow mainWin;
};

int main(int argc, char *argv[])
{
    return App{argc, argv}.start();
}
