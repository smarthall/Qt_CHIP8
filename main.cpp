#include "emulatorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EmulatorWindow w;
    w.show();

    return a.exec();
}
