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

    void moveNode(QGraphicsSceneMouseEvent *event);
    void addEdge(Edge *edge, bool startsFromThisNode);
    void deleteEdge(Node *otherEnd);
    void removeEdgeFromList(Edge *edge);
//    void adjustEdges();

    void setBorder(const bool &hasBorder);
    void setActive(const bool &active = true);
    void setEditable(const bool &editable = true);
    void setColor(const QColor &color);
    QColor color() const { return m_color; }
    void setTextColor(const QColor &color);
    QColor textColor() const { return m_textColor; }
    void setScale(const qreal &factor, const QRectF &sceneRect);

    void showNumber(const int &number, const bool& show = true,
                    const bool &numberIsSpecial = false);
    void insertPicture(const QString &picture, const int &pos = 0);
    double calculateBiggestAngle();

    // changing visibility from prot to pub
    void keyPressEvent(QKeyEvent *event);
    bool isConnected(const Node *node) const;
    QPointF intersect(const QLineF &line, const bool &reverse = false) const;

    QList<Edge *> edgesFrom(const bool &excludeSecondaries = true) const;
    QList<Edge *> edgesToThis(const bool &excludeSecondaries = true) const;
    Edge * edgeTo(const Node* node) const;
    QList<Node *> subtree() const;

protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QPainterPath shape () const;

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
    QColor m_color;
    QColor m_textColor;

    static const double m_pi;
    static const double m_oneAndHalfPi;
    static const double m_twoPi;

    static const QColor m_orange;
    static const QColor m_gold;
};

#endif // NODE_H
