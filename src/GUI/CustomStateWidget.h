//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QHBoxLayout>
#include "CustomCircleGraph.h"
#include <tinyxml2.h>
using namespace tinyxml2;

class CustomStateWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomStateWidget(QWidget *parent = nullptr);
    /**
     * 将stateMachine内配置信息显示出来
     * @param stateMachineConf
     */
    void setConf(XMLElement *stateMachineConf);
    /**
     * 将widget中的配置更新到stateMachineElement中
     * @param stateMachineElement
     */
    void saveConfToXML(XMLElement *stateMachineElement);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QHBoxLayout *vBoxLayout;

    QGraphicsView *canvasView;
    QGraphicsScene *canvasScene;

    QGraphicsView *graphBoardView;
    QGraphicsScene *graphBoardScene;
    CustomCircleGraph *circleItem;
    QGraphicsPixmapItem *circlePixmapItem;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H
