#include <QPainter>
#include <QDebug>


#include "edge.h"
#include "node.h"

#include <math.h>

const double Edge::m_pi = 3.14159265358979323846264338327950288419717;
const double Edge::m_twoPi = 2.0 * Edge::m_pi;
const qreal Edge::m_arrowSize = 7;

Edge::Edge(Node *sourceNode, Node *destNode)
    : m_angle(-1)
    , m_color(0,0,0)
    , m_width(1)
    , m_secondary(false)
{
    setAcceptedMouseButtons(0);
    setZValue(1);

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

double Edge::getAngle() const
{
    return m_angle;
}

void Edge::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void Edge::setWidth(const qreal &width)
{
    if (width < 1 || width > 100)
        return;

    m_width = width;
    update();
}

void Edge::setSecondary(const bool &sec)
{
    m_secondary = sec;
    update();
}

void Edge::adjust()
{
    if (!m_sourceNode || !m_destNode)
        return;

    prepareGeometryChange();

    QLineF line(m_sourceNode->sceneBoundingRect().center(),
                m_destNode->sceneBoundingRect().center());

    if (line.length() > qreal(20.))
    {
//        m_sourcePoint = m_sourceNode->intersect(line);
        m_destPoint = m_destNode->intersect(line,true);

        /// @bug seems not to be updated
        m_sourcePoint = m_sourceNode->sceneBoundingRect().center();
//        m_destPoint = m_destNode->sceneBoundingRect().center();

    } else {
        m_sourcePoint = m_destPoint = line.p1();
    }
}

/// @bug not ok yet
QRectF Edge::boundingRect() const
{
    if (!m_sourceNode || !m_destNode)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize  + m_width) / 2.0;

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
        m_angle = Edge::m_twoPi - m_angle;

    if (sourceNode()->collidesWithItem(destNode()))
        return;

    // Draw the line itself
    painter->setPen(QPen(m_color,
                         m_width,
                         m_secondary ?
                             Qt::DashLine :
                             Qt::SolidLine,
                         Qt::RoundCap,
                         Qt::RoundJoin));
    painter->drawLine(line);

    if (line.length() < m_arrowSize)
        return;

    // Draw the arrow
    painter->setPen(QPen(m_color,
                         m_width,
                         Qt::SolidLine,
                         Qt::RoundCap,
                         Qt::RoundJoin));

    qreal arrowSize = m_arrowSize + m_width;

    QPointF destArrowP1 = m_destPoint +
                          QPointF(sin(m_angle - Edge::m_pi / 3) * arrowSize,
                                  cos(m_angle - Edge::m_pi / 3) * arrowSize);
    QPointF destArrowP2 = m_destPoint +
              QPointF(sin(m_angle - Edge::m_pi + Edge::m_pi / 3) * arrowSize,
                      cos(m_angle - Edge::m_pi + Edge::m_pi / 3) * arrowSize);
    painter->setBrush(m_color);
    painter->drawPolygon(QPolygonF() << line.p2()
                                     << destArrowP1
                                     << destArrowP2);
}
