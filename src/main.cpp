#include <QApplication>
#include <QtWidgets/QMainWindow>
#include "GUI/CustomMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CustomMainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}


