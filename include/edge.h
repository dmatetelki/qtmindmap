#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

// directed arrow
class Edge : public QGraphicsItem
{
public:

    Edge(Node *sourceNode, Node *destNode);
    ~Edge();

    Node *sourceNode() const { return m_sourceNode; }
    Node *destNode() const { return m_destNode; }
    double angle() const { return m_angle; }

    // set/get color/width/secondary
    QColor color() const { return m_color; }
    void setColor(const QColor &color);
    qreal width() const { return m_width; }
    void setWidth(const qreal &width);
    bool secondary() const  { return m_secondary; }
    void setSecondary(const bool &sec = true );

    // re-calculates the source and endpoint.
    // called when the source/dest node changed (size,pos)
    void adjust();

protected:

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:

    Node *m_sourceNode;
    Node *m_destNode;

    QPointF m_sourcePoint;
    QPointF m_destPoint;
    double m_angle;
    QColor m_color;
    qreal m_width;

    // just a logical connection between two nodes,
    // does not counts at subtree calculation
    bool m_secondary;

    static const qreal m_arrowSize;
    static const double m_pi;
    static const double m_twoPi;
};

#endif
