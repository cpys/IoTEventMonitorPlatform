//
// Created by yingzi on 2017/10/9.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMCIRCLEGRAPH_H
#define IOTEVENTMONITORPLATFORM_CUSTOMCIRCLEGRAPH_H


#include <QtWidgets/QGraphicsItem>
#include <QPainter>
#include "ConstStyle.h"

class CustomCircleGraph : public QObject, public QGraphicsItem {
  Q_OBJECT

  public:
    explicit CustomCircleGraph(QObject *parent = nullptr);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMCIRCLEGRAPH_H
