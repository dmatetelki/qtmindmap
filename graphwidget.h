#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "node.h"


class GraphWidget : public QGraphicsView
{
    Q_OBJECT


public:
    GraphWidget(QWidget *parent = 0);
    QGraphicsScene *getScene();

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);

private:
    Node *activeNode;
    QGraphicsScene *scene;

};

#endif // GRAPHWIDGET_H
