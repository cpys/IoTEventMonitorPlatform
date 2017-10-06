#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include "CustomMainTabWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->resize(1000, 600);
    mainWindow->setCentralWidget(new CustomMainTabWidget(mainWindow));
    mainWindow->setStatusBar(new QStatusBar());
    mainWindow->setContentsMargins(0, 0, 0, 0);
    mainWindow->show();

    return a.exec();
}
