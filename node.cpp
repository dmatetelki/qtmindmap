#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

Node::Node(GraphWidget *parent) : graph(parent), active(false)
{
    qDebug() << __PRETTY_FUNCTION__;

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(2);

    // shall I set it after some spec key?
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void Node::addEdge(Edge *edge)
{
    qDebug() << __PRETTY_FUNCTION__;

    edgeList << edge;
    edge->adjust();
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << __PRETTY_FUNCTION__;

    switch (change) {
        case ItemPositionHasChanged:
            foreach (Edge *edge, edgeList) edge->adjust();
            //         graph->itemMoved();
            break;
        default:
            break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

//    active = true;
//    setScale(1.2);
    update();
    QGraphicsTextItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

//    active = false;
//    setScale(1.0);
    update();
    QGraphicsTextItem::mouseReleaseEvent(event);
}

//void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    qDebug() << __PRETTY_FUNCTION__;
//    QGraphicsTextItem::paint(painter, option, widget);

//    qDebug() << "I " << (hasFocus() ? "have " : "don't have ") << "focus.";
//    setScale(hasFocus() ? 1.2 : 1.0);
////    setScale(active ? 1.2 : 1.0);
//}
