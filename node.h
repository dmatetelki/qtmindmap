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

    void addEdge(Edge *edge, bool startsFromThisNode);
    void removeEdge(Edge *edge);
//    QList<Edge *> edges() const;
    void setBorder(const bool &hasBorder);
    void setActive(const bool &active = true);
    void showNumber(const int &number, const bool& show = true, const bool &numberIsSpecial = false);
    double calculateBiggestAngle();

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

    struct EdgeElement
    {
        Edge *edge;
        bool startsFromThisNode;
        EdgeElement(Edge *e, bool s) : edge(e), startsFromThisNode(s) {}
    };
//    class EdgeList : public QList<EdgeElement>
//    {
//    public:
//        int compareItems(QCollection::Item a, QCollection::Item b)
//        {
//            return (EdgeElement)a.edge = (EdgeElement)b.edge;
//        }
//    };

    QList<EdgeElement> m_edgeList;
    GraphWidget *m_graph;
    bool m_isActive;
//    Edge *m_activeEdge;
    int m_number;
    bool m_hasBorder;
    bool m_numberIsSpecial;
};

#endif // NODE_H
