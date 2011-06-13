#include "node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>

static const double Pi = 3.14159265358979323846264338327950288419717;

Node::Node(GraphWidget *parent) :
    m_graph(parent),
    m_isActive(false),
    m_number(-1),
    m_hasBorder(true),
    m_numberIsSpecial(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);

    setCacheMode(DeviceCoordinateCache);

    // shall I use system colors?
    setDefaultTextColor(QColor(0,0,0));
}

Node::~Node()
{
    foreach (EdgeElement element, m_edgeList) delete element.edge;
}

void Node::addEdge(Edge *edge, bool startsFromThisNode)
{
    m_edgeList.push_back(EdgeElement(edge, startsFromThisNode));
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        if (it->edge == edge)
        {
            m_edgeList.erase(it);
            return;
        }
    }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
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
    // draw background in hint mode
    /// @bug is there a 1pixel wide yellow line at the bottom of borderless item?
    if (m_number != -1)
    {
        painter->setPen(Qt::transparent);
        painter->setBrush(m_numberIsSpecial ? Qt::green : Qt::yellow);
        painter->drawRect(QRect(boundingRect().topLeft().toPoint(),
                          boundingRect().bottomRight().toPoint()));
        painter->setBrush(Qt::NoBrush);
    }

    QGraphicsTextItem::paint(painter, option, w);

    painter->setPen(m_isActive ? Qt::red : Qt::blue);

    if (m_hasBorder)
        painter->drawRect(QRect(boundingRect().topLeft().toPoint(),
                          boundingRect().bottomRight().toPoint() -
                          QPoint(1,1)));

    if (m_number != -1)
    {
        painter->setPen(Qt::white);
        painter->setBackground(Qt::red);
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->drawText(boundingRect().topLeft()+QPointF(0,11),
                          QString("%1").arg(m_number));
    }
}



void Node::setActive(const bool &active)
{
    m_isActive = active;
    update();
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_graph->setActiveNode(this);

    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_graph->setActiveNodeEditable();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void Node::showNumber(const int &number,
                      const bool& show,
                      const bool &numberIsSpecial)
{
    m_number = show ? number : -1;
    m_numberIsSpecial = numberIsSpecial;
    update();
}

void Node::setBorder(const bool &hasBorder)
{
   m_hasBorder = hasBorder;
   update();
}

double Node::calculateBiggestAngle()
{
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
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        tmp.push_back(it->startsFromThisNode ?
                          it->edge->getAngle() :
                          doubleModulo(Pi + it->edge->getAngle(), 2 * Pi));
    }

    qSort(tmp.begin(), tmp.end());

    double prev(tmp.first());
    double max_prev(tmp.last());
    double max(2 * Pi - tmp.last() + tmp.first());

    for(QList<double>::const_iterator it = ++tmp.begin(); it!=tmp.end(); it++)
    {
        if (*it - prev > max )
        {
            max = *it - prev;
            max_prev = prev;
        }
        prev = *it;
    }

    return 2 * Pi - doubleModulo(max_prev + max / 2, 2 * Pi);
}

void Node::linkActivated(const QString &link)
{
	qDebug() << __PRETTY_FUNCTION__;

	qDebug() << link;
}

double Node::doubleModulo(const double &devided, const double &devisor)
{
    return devided - static_cast<double>(devisor * static_cast<int>(devided / devisor));
}

void Node::setEditable(const bool &editable)
{
    qDebug() << __PRETTY_FUNCTION__;

    setTextInteractionFlags(
                editable ?
                    Qt::TextEditable :
                    Qt::NoTextInteraction);

    QTextCursor c = textCursor();
    c.setPosition(c.document()->toPlainText().length());
    setTextCursor(c);
}

void Node::keyPressEvent(QKeyEvent *event)
{
    // cursor movements
    switch (event->key()) {

    case Qt::Key_Left:
    {
        QTextCursor c = textCursor(); // textcursor() return just a copy
        c.movePosition(
                    event->modifiers() == Qt::ControlModifier ?
                    QTextCursor::PreviousWord :
                    QTextCursor::PreviousCharacter);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Right:
    {
        QTextCursor c = textCursor();
        c.movePosition(
                    event->modifiers() == Qt::ControlModifier ?
                    QTextCursor::NextWord :
                    QTextCursor::NextCharacter);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Up:
    {
        QTextCursor c = textCursor();
        c.movePosition(QTextCursor::Up);
        setTextCursor(c);
        break;
    }
    case Qt::Key_Down:
    {
        QTextCursor c = textCursor();
        c.movePosition(QTextCursor::Down);
        setTextCursor(c);
        break;
    }

    default:

        // not cursor movement
        QGraphicsTextItem::keyPressEvent(event);
        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
    }

    ///@note leaving editing mode is done with esc, handled by graphwidget
}
