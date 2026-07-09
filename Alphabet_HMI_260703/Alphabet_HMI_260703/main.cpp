#include "HMI_main.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    HMI_main window;
    window.show();
    return app.exec();
}
