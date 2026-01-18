#include <QApplication>
#include "interface.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Interface mainWindow;
    mainWindow.show();

    return app.exec();
}
