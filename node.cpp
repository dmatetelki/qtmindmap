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
//    setZValue(1);

    // shall I use system colors?
    setDefaultTextColor(QColor(0,0,0));

    // shall I set it after some spec key?
//    setTextInteractionFlags(Qt::TextEditorInteraction);
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

    update();
    QGraphicsTextItem::mouseReleaseEvent(event);
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w)
{
        qDebug() << __PRETTY_FUNCTION__;

        QGraphicsTextItem::paint(painter, option, w);

        QPen pen(Qt::blue,1);
//        pen.setJoinStyle(Qt::RoundJoin);
//        pen.setStyle(Qt::MiterJoin);
//        pen.setCapStyle(Qt::RoundCap);
//        pen.setMiterLimit(3);
        painter->setPen(pen);

//        painter->setPen(QPen(Qt::blue
//                             , 1, Qt::SolidLine,Qt::SquareCap, Qt::RoundJoin
//                             ));
        m_rect = QRect(	boundingRect().topLeft().toPoint()
//                        - QPoint(4,4)
                        ,
                        boundingRect().bottomRight().toPoint()
                        - QPoint(1,1)
                        );
        painter->drawRect(m_rect);
}
