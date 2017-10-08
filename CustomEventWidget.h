//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QGridLayout>

class CustomEventWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomEventWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QGridLayout *gridLayout;

    QTextEdit *upperOutLayer;
    QTextEdit *middleLayer;
    QTextEdit *bottomOutLayer;
    QTextBrowser *upperIndicator;
    QTextBrowser *middleIndicator;
    QTextBrowser *bottomIndicator;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H
