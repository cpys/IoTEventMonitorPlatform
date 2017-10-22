#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include "GUI/CustomMainTabWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow *mainWindow = new QMainWindow();

    mainWindow->setCentralWidget(new CustomMainTabWidget(mainWindow));
    mainWindow->setStatusBar(new QStatusBar());
    mainWindow->resize(1000, 600);
    mainWindow->setContentsMargins(0, 0, 0, 0);

    QObject::connect(mainWindow->centralWidget(), SIGNAL(sendStatusMessage(const QString&)), mainWindow->statusBar(), SLOT(showMessage(const QString&)));

    mainWindow->show();

    return a.exec();
}


