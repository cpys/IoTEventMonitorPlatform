//
// Created by yingzi on 2017/10/23.
//

#include "CustomMainWindow.h"

CustomMainWindow::CustomMainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new CustomMainTabWidget(this);
    statusBar = new QStatusBar();

    this->setCentralWidget(centralWidget);
    this->setStatusBar(statusBar);

    this->resize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    this->setContentsMargins(0, 0, 0, 0);
    this->setWindowTitle("物联网服务系统监控平台");

    QObject::connect(centralWidget, SIGNAL(sendStatusMessage(const QString&)), statusBar, SLOT(showMessage(const QString&)));
}

void CustomMainWindow::closeEvent(QCloseEvent *event) {
    centralWidget->saveConf();
    QWidget::closeEvent(event);
}


