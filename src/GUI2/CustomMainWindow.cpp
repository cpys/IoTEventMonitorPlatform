//
// Created by chenkuan on 2018/2/20.
//

#include "CustomMainWindow.h"
#include <GUIStyle.h>

CustomMainWindow::CustomMainWindow(QWidget *parent) : QMainWindow(parent) {
    tabWidget = new CustomTabWidget(this);
    statusBar = new QStatusBar(this);

    setCentralWidget(tabWidget);
    setStatusBar(statusBar);

    setContentsMargins(0, 0, 0, 0);
    resize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    setWindowTitle(MAIN_WINDOW_TITLE);
}