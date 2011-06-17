#ifndef NODE_H
#define NODE_H

#include <QGraphicsTextItem>
#include <QTextCursor>

#include "edge.h"
#include "graphwidget.h"

class GraphWidget;

class Node : public QGraphicsTextItem
{
public:

    Node(GraphWidget *graphWidget = 0);
    ~Node();

    void addEdge(Edge *edge, bool startsFromThisNode);
    void deleteEdge(Node *otherEnd);
    void removeEdgeFromList(Edge *edge);

    void setBorder(const bool &hasBorder);
    void setActive(const bool &active = true);
    void setEditable(const bool &editable = true);

    void showNumber(const int &number, const bool& show = true,
                    const bool &numberIsSpecial = false);
    double calculateBiggestAngle();

    // changing visibility from prot to pub
    void keyPressEvent(QKeyEvent *event);
    bool isConnected(const Node *node) const;

    QList<Edge *> edgesFrom() const;

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

    double doubleModulo(const double &devided, const double &devisor);

    struct EdgeElement
    {
        Edge *edge;
        bool startsFromThisNode;
        EdgeElement(Edge *e, bool s) : edge(e), startsFromThisNode(s) {}
    };

    QList<EdgeElement> m_edgeList;
    GraphWidget *m_graph;
    bool m_isActive;
    int m_number;
    bool m_hasBorder;
    bool m_numberIsSpecial;

    static const double m_pi;
    static const double m_oneAndHalfPi;
    static const double m_twoPi;

    static const QColor m_orange;
};

#endif // NODE_H
