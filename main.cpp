#include <QApplication>

#include "mainwindow.h"
#include "connect.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<GameProcess>("GameProcess");

    app.setOrganizationName("JSteward Tech");
    app.setApplicationName("ataxx");

    if (!createConnection())
        return 1;

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
