//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_RUNWIDGET_H
#define IOTEVENTMONITORPLATFORM_RUNWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include "CustomIpEdit.h"

/**
 * 运行展示窗口Widget的重写
 */
class CustomRunWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomRunWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QGridLayout *gridLayout = nullptr;
    QComboBox *eventComboBox = nullptr;
    QComboBox *stateComboBox = nullptr;
    QTextBrowser *eventTextBrowser = nullptr;
    QGraphicsView *stateGraphicsView = nullptr;
    CustomIpEdit *sourceIpEdit = nullptr;
    CustomIpEdit *destIpEdit = nullptr;
    QLineEdit *pseudoTerminalEdit = nullptr;
    QLineEdit *serialPortEdit = nullptr;
    QLineEdit *vmEdit = nullptr;
    QPushButton *runButton = nullptr;
    QTextBrowser *eventTraceTextBrowser = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_RUNWIDGET_H
