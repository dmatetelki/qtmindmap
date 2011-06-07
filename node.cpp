#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Node::Node(GraphWidget *parent) : m_graph(parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
//    setZValue(1);

    // shall I use system colors?
    setDefaultTextColor(QColor(0,0,0));

    // shall I set it after some spec key?
//    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void Node::addEdge(Edge *edge)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_edgeList << edge;
    edge->adjust();
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << __PRETTY_FUNCTION__;

    switch (change) {

    case ItemPositionChange:

        if (change == ItemPositionChange && scene()) {
            // value is the new position.
            QPointF newPos = value.toPointF();
            QRectF rect (scene()->sceneRect().topLeft(),
                         scene()->sceneRect().bottomRight()-boundingRect().bottomRight());

            if (!rect.contains(newPos))
            {
                // Keep the item inside the scene rect.
                newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                return newPos;
            }
        }
        break;

    case ItemPositionHasChanged:

        foreach (Edge *edge, m_edgeList) edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
        qDebug() << __PRETTY_FUNCTION__;

        QGraphicsTextItem::paint(painter, option, w);

        painter->setPen(QPen(Qt::blue));
        painter->drawRect(QRect(boundingRect().topLeft().toPoint(),
                                boundingRect().bottomRight().toPoint() -
                                QPoint(1,1)));
}
