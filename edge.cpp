#include <QPainter>
#include <QDebug>


#include "edge.h"
#include "node.h"

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode)
    : m_arrowSize(7),
      m_angle(-1)
{
    setAcceptedMouseButtons(0);
    m_sourceNode = sourceNode;
    m_destNode = destNode;
    m_sourceNode->addEdge(this,true);
    m_destNode->addEdge(this,false);
    adjust();
}

Edge::~Edge()
{
    m_sourceNode->removeEdgeFromList(this);
    m_destNode->removeEdgeFromList(this);
}

Node *Edge::sourceNode() const
{
    return m_sourceNode;
}

Node *Edge::destNode() const
{
    return m_destNode;
}

/** @note This is brute force. Isn't there a simple fv for this?
  * The precision is not the best either
  */
QPointF firstNotContainedPoint(const QLineF &line,
                               const QRectF &rect,
                               bool reverse = false)
{
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

    QLineF line(mapFromItem(m_sourceNode, 0, 0) +
                m_sourceNode->boundingRect().center(),
                mapFromItem(m_destNode, 0, 0)  +
                m_destNode->boundingRect().center());

    if (line.length() > qreal(20.)) {
        m_sourcePoint = firstNotContainedPoint(line,
                            m_sourceNode->sceneBoundingRect());
        m_destPoint = firstNotContainedPoint(line,
                            m_destNode->sceneBoundingRect(),true);
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
        .normalized().adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *w)
{
    Q_UNUSED(w);

    if (!m_sourceNode || !m_destNode)
        return;

    QLineF line(m_sourcePoint, m_destPoint);

    m_angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        m_angle = TwoPi - m_angle;

    if (sourceNode()->collidesWithItem(destNode()))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black,
                         1,
                         Qt::SolidLine,
                         Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawLine(line);

    if (line.length() < m_arrowSize)
        return;

    // Draw the arrows
    QPointF destArrowP1 = m_destPoint +
                             QPointF(sin(m_angle - Pi / 3) * m_arrowSize,
                                     cos(m_angle - Pi / 3) * m_arrowSize);
    QPointF destArrowP2 = m_destPoint +
                             QPointF(sin(m_angle - Pi + Pi / 3) * m_arrowSize,
                                     cos(m_angle - Pi + Pi / 3) * m_arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2()
                                     << destArrowP1
                                     << destArrowP2);
}

double Edge::getAngle() const
{
    return m_angle;
}
