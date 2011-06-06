#include <QPainter>
#include <QDebug>


#include "edge.h"
#include "node.h"

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode)
    : m_arrowSize(7)
{
    setAcceptedMouseButtons(0);
    m_sourceNode = sourceNode;
    m_destNode = destNode;
    m_sourceNode->addEdge(this);
    m_destNode->addEdge(this);
    adjust();
//    setZValue(1);
}

Node *Edge::sourceNode() const
{
    return m_sourceNode;
}

Node *Edge::destNode() const
{
    return m_destNode;
}

/// @note This is brute force. Isn't there a simple fv for this?
QPointF firstNotContainedPoint(const QLineF &line,
                               const QPointF &pos,
                               const QRectF &rectangle,
                               bool reverse = false)
{
    QRectF rect(rectangle.topLeft()+pos, rectangle.bottomRight()+pos);
    if (reverse)
    {
        for (qreal t = 1; t!=0; t-=0.01)
        {
            if (!rect.contains(line.pointAt(t))) return line.pointAt(t);
        }
    }
    else
    {
        for (qreal t = 0; t!=1; t+=0.01)
        {
            if (!rect.contains(line.pointAt(t))) return line.pointAt(t);
        }
    }
    return QPoint(0,0);
}

void Edge::adjust()
{
    if (!m_sourceNode || !m_destNode)
        return;


    prepareGeometryChange();

    QLineF line(mapFromItem(m_sourceNode, 0, 0) + m_sourceNode->boundingRect().center(),
                mapFromItem(m_destNode, 0, 0)  + m_destNode->boundingRect().center());
    qreal length = line.length();



    if (length > qreal(20.)) {

        QPointF sourceOffset(firstNotContainedPoint(line,
                                                    m_sourceNode->pos(),
                                                    m_sourceNode->boundingRect()
                                                    ));
        QPointF destOffset(firstNotContainedPoint(line,
                                                  m_destNode->pos(),
                                                  m_destNode->boundingRect()
                                                  ,true));

        m_sourcePoint = sourceOffset;
        m_destPoint = destOffset;
    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!m_sourceNode || !m_destNode)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize) / 2.0;

    return QRectF(m_sourcePoint, QSizeF(m_destPoint.x() - m_sourcePoint.x(),
                                      m_destPoint.y() - m_sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_sourceNode || !m_destNode)
        return;

    QLineF line(m_sourcePoint, m_destPoint);

    if (sourceNode()->collidesWithItem(destNode()))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    if (line.length() < m_arrowSize)
        return;

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = m_destPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize,
                                              cos(angle - Pi / 3) * m_arrowSize);
    QPointF destArrowP2 = m_destPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize,
                                              cos(angle - Pi + Pi / 3) * m_arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
