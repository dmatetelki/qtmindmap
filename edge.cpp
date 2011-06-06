#include <QPainter>
#include <QDebug>


#include "edge.h"
#include "node.h"

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
//    setZValue(1);
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
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
    if (!source || !dest)
        return;


    prepareGeometryChange();

    QLineF line(mapFromItem(source, 0, 0) + source->boundingRect().center(),
                mapFromItem(dest, 0, 0)  + dest->boundingRect().center());
    qreal length = line.length();



    if (length > qreal(20.)) {

        QPointF sourceOffset(firstNotContainedPoint(line,
                                                    source->pos(),
                                                    source->boundingRect()
                                                    ));
        QPointF destOffset(firstNotContainedPoint(line,
                                                  dest->pos(),
                                                  dest->boundingRect()
                                                  ,true));

        sourcePoint = sourceOffset;
        destPoint = destOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);

    /// @bug this test does not filter out when the nodes intersect
//    if (qFuzzyCompare(line.length(), qreal(0.)))
//        return;

    if (sourceNode()->collidesWithItem(destNode()))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    if (line.length() < qreal(10.))
        return;

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

//    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
//                                                  cos(angle + Pi / 3) * arrowSize);
//    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
//                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
//    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
