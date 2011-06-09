#ifndef NODE_H
#define NODE_H

#include <QGraphicsTextItem>

#include "edge.h"
#include "graphwidget.h"

class GraphWidget;

class Node : public QGraphicsTextItem
{
public:

    Node(GraphWidget *graphWidget = 0);
    ~Node();

    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
//    QList<Edge *> edges() const;
    void setActive(const bool &active = true);
    void showNumber(const int &number, const bool& show = true, const bool &numberIsSpecial = false);


protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

    QList<Edge *> m_edgeList;
    GraphWidget *m_graph;
    bool m_isActive;
//    Edge *m_activeEdge;
    int m_number;
    bool m_numberIsSpecial;
};

#endif // NODE_H
