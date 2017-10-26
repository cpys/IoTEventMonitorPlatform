//
// Created by yingzi on 2017/10/9.
//

#include "CustomCircleGraph.h"

CustomCircleGraph::CustomCircleGraph(QObject *parent) : QObject(parent) {
    this->setFlags(QGraphicsItem::ItemIsMovable);
    this->setFlags(QGraphicsItem::ItemIsSelectable);
}

QRectF CustomCircleGraph::boundingRect() const {
    return QRectF();
}

void CustomCircleGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->drawEllipse(QRect(-CIRCLE_RADIUS / 2, - CIRCLE_RADIUS / 2, CIRCLE_RADIUS, CIRCLE_RADIUS));
}
