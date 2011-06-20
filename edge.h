#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:

    Edge(Node *sourceNode, Node *destNode);
    ~Edge();

    Node *sourceNode() const;
    Node *destNode() const;
    double getAngle() const;

    QColor color() const { return m_color; }
    void setColor(const QColor &color);
    qreal width() const { return m_width; }
    void setWidth(const qreal &width);
    bool secondary() const  { return m_secondary; }
    void setSecondary(const bool &sec = true );

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
    bool m_secondary;

    static const qreal m_arrowSize;
    static const double m_pi;
    static const double m_twoPi;
};

#endif
