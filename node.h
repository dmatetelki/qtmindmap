#ifndef NODE_H
#define NODE_H

#include <QGraphicsTextItem>

#include "edge.h"
#include "graphwidget.h"

class GraphWidget;

class Node : public QGraphicsTextItem
{
//    Q_OBJECT

public:
    Node(GraphWidget *graphWidget = 0);

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QList<Edge *> edgeList;
    GraphWidget *graph;
    bool active;

};

#endif // NODE_H
