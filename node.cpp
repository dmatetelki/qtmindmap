#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;

Node::Node(GraphWidget *parent) :
    m_graph(parent),
    m_isActive(false),
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
    foreach (EdgeElement element, m_edgeList) delete element.edge;
}

void Node::addEdge(Edge *edge, bool startsFromThisNode)
{
    qDebug() << __PRETTY_FUNCTION__;

    m_edgeList.push_back(EdgeElement(edge, startsFromThisNode));
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    qDebug() << __PRETTY_FUNCTION__;

    for(QList<EdgeElement>::iterator it = m_edgeList.begin(); it != m_edgeList.end(); it++)
        if (it->edge == edge)
        {
            m_edgeList.erase(it);
            return;
        }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
//    qDebug() << __PRETTY_FUNCTION__;

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

        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
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
//    qDebug() << __PRETTY_FUNCTION__;

    QGraphicsItem::mouseMoveEvent(event);
}

void Node::showNumber(const int &number, const bool& show, const bool &numberIsSpecial)
{
//    qDebug() << __PRETTY_FUNCTION__;

    m_number = show ? number : -1;
    m_numberIsSpecial = numberIsSpecial;
    update();
}

double Node::calculateBiggestAngle()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (m_edgeList.empty())
        return 1.5 * Pi;

    if (m_edgeList.size()==1)
    {
        if (m_edgeList.first().startsFromThisNode)
        {
            return Pi - m_edgeList.first().edge->getAngle();
        }
        else
        {
            return 2 * Pi - m_edgeList.first().edge->getAngle();
        }
    }

    QList<double> tmp;
    for(QList<EdgeElement>::iterator it = m_edgeList.begin(); it != m_edgeList.end(); it++)
    {
        if (it->startsFromThisNode)
        {
            tmp.push_back(Pi - it->edge->getAngle());
        }
        else
        {
            tmp.push_back(2 * Pi - it->edge->getAngle());
        }
    }

    qSort(tmp.begin(), tmp.end());

    qDebug() << tmp;

    double prev(tmp.last());
    double max_prev(tmp.last());
    double max(0);

    /// @bug algorith is baaad
    for(QList<double>::const_iterator it = tmp.begin(); it!=tmp.end(); it++)
    {
        if (abs(*it - prev) > abs(max) )
        {
            max = *it - prev;
            max_prev = prev;
        }
        prev = *it;
    }

    qDebug() << max;
    qDebug() << max_prev;

    return max_prev + max / 2 ;
}
