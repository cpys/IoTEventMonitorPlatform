//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QGridLayout>
#include <tinyxml2.h>
using namespace tinyxml2;

class CustomEventWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomEventWidget(QWidget *parent = nullptr);
    /**
     * 将event内配置信息显示出来
     * @param eventConf
     */
    void setConf(XMLElement *eventConf);
    /**
     * 将widget中的配置更新到eventElement中
     * @param eventElement
     */
    void saveConfToXML(XMLElement *eventElement);

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
