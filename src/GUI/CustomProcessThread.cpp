
#include <QtCore/QThread>
#include <iostream>
#include "CustomProcessThread.h"

void CustomProcessThread::stop() {
    threadStop = true;
}

void CustomProcessThread::run() {
    // TODO
    threadStop = false;

    while (!threadStop) {
        sleep(1);
    }
}
