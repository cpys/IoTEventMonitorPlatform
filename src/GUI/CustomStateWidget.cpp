//
// Created by yingzi on 2017/10/6.
//

#include "CustomStateWidget.h"

CustomStateWidget::CustomStateWidget(QWidget *parent) : QWidget(parent) {
//    vBoxLayout = new QHBoxLayout(this);
//    canvasView = new QGraphicsView(this);
//    canvasScene = new QGraphicsScene();
//    graphBoardView = new QGraphicsView(this);
//    graphBoardScene = new QGraphicsScene();
//    circleItem = new CustomCircleGraph;
//    circlePixmapItem = new QGraphicsPixmapItem(QPixmap("circle.png"));
//
//    circlePixmapItem->setFlags(QGraphicsItem::ItemIsMovable);
////    circlePixmapItem->setFlags(QGraphicsItem::ItemIsSelectable);
//
//    vBoxLayout->addWidget(canvasView, 8);
//    vBoxLayout->addWidget(graphBoardView, 2);
//    vBoxLayout->setContentsMargins(0, 0, 0, 0);
//    vBoxLayout->setSpacing(0);
//
////    graphBoardScene->addItem(circleItem);
//    graphBoardScene->addItem(circlePixmapItem);
//
//    canvasView->setScene(canvasScene);
//    graphBoardView->setScene(graphBoardScene);

    gridLayout = new QGridLayout(this);

    webView = new QWebEngineView(this);
    webView->load(QUrl("http://localhost:8080/javascript/examples/grapheditor/www/index.html"));

    gridLayout->addWidget(webView);
}

void CustomStateWidget::setConf(XMLElement *stateMachineConf) {
    // TODO
}

void CustomStateWidget::saveConfToXML(XMLElement *stateMachineElement) {
    // TODO
}
