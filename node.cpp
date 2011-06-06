#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

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
        case ItemPositionHasChanged:

            foreach (Edge *edge, m_edgeList) edge->adjust();
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    update();
    QGraphicsTextItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    update();
    QGraphicsTextItem::mouseReleaseEvent(event);
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
