#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

Node::Node(GraphWidget *parent) :
    m_graph(parent),
    m_isActive(false),
//    m_activeEdge(0),
    m_number(-1),
    m_numberIsSpecial(false)
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

Node::~Node()
{
    qDebug() << __PRETTY_FUNCTION__;
    foreach (Edge *edge, m_edgeList) delete edge;
}

void Node::addEdge(Edge *edge)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_edgeList << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_edgeList.removeAll(edge);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << __PRETTY_FUNCTION__;

    switch (change) {

    case ItemPositionChange:

        if (change == ItemPositionChange && scene())
        {
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
//    qDebug() << __PRETTY_FUNCTION__;

    if (m_number != -1)
    {
        painter->setBackground(m_numberIsSpecial ? Qt::green : Qt::yellow);
        painter->setBackgroundMode(Qt::OpaqueMode);
    }

    QGraphicsTextItem::paint(painter, option, w);

    painter->setPen(m_isActive ? Qt::red : Qt::blue);

    painter->drawRect(QRect(boundingRect().topLeft().toPoint(),
                            boundingRect().bottomRight().toPoint() -
                            QPoint(1,1)));

    if (m_number != -1)
    {
        painter->setPen(Qt::white);
        painter->setBackground(Qt::red);
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->drawText(boundingRect().topLeft()+QPointF(0,11), QString("%1").arg(m_number));
    }
}

void Node::setActive(const bool &active)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_isActive = active;
    update();
}


/// @note who shall set active: press or release?
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_graph->setActiveNode(this);

    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_graph->insertNode();

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    QGraphicsItem::mouseMoveEvent(event);
}

void Node::showNumber(const int &number, const bool& show, const bool &numberIsSpecial)
{
//    qDebug() << __PRETTY_FUNCTION__;

    m_number = show ? number : -1;
    m_numberIsSpecial = numberIsSpecial;
    update();
}
