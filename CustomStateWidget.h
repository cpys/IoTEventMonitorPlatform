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

class CustomStateWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomStateWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QHBoxLayout *vBoxLayout;

    QGraphicsView *canvasView;
    QGraphicsScene *canvasScene;

    QGraphicsView *graphBoardView;
    QGraphicsScene *graphBoardScene;
    CustomCircleGraph *circleItem;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H
