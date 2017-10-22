//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
#define IOTEVENTMONITORPLATFORM_STATETABWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "CustomStateWidget.h"
#include "CustomSubTabWidget.h"

/**
 * 状态机窗口TabWidget的重写
 */
class CustomStateTabWidget : public CustomSubTabWidget {
  Q_OBJECT

  public:
    /**
     * 构造函数中需要添加默认的stateWidget
     * @param parent
     */
    explicit CustomStateTabWidget(QWidget *parent = nullptr);

  protected:
    void addCustomTab() override ;

  protected slots:
    void saveCurrentTab() override ;

  private:
    QPushButton *viewXMLButton;

  private slots:
    void viewCurrentXML();
};


#endif //IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
