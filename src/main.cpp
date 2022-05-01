#include <QApplication>
#include "MainWindow.h"
#include "services/DebugService.h"
#include "services/CodeService.h"

Semantics* Semantics::instance = nullptr;
DebugService* DebugService::instance = nullptr;
CodeService* CodeService::instance = nullptr;

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

int main(int argc, char* argv[])
{
    return App{ argc, argv }.start();
}
