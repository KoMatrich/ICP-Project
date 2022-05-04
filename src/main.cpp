#include <QApplication>
#include "MainWindow.h"

/*!
 * @brief Main application class
*/
class App
{
public:
    /*!
     * @brief        Creates new application
     * @param argc - Number of arguments
     * @param argv - Arguments
    */
    App(int& argc, char** argv)
        :app(argc, argv)
    {
        app.setApplicationName("ICP-Project");
        app.setOrganizationName("BUT FIT");
    }
    /// Starts application
    int start()
    {
        mainWin.show();
        return app.exec();
    }
private:
    /// Application
    QApplication app;
    /// Application window
    MainWindow mainWin;
};

/*!
 * @brief        Main entry of program
 * @param argc - Number of arguments
 * @param argv - Arguments
 * @return       Aplicaton return code
*/
int main(int argc, char* argv[])
{
    return App{ argc, argv }.start();
}
