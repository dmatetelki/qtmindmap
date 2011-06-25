#include "include/node.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>

const double Node::m_pi = 3.14159265358979323846264338327950288419717;
const double Node::m_oneAndHalfPi = Node::m_pi * 1.5;
const double Node::m_twoPi = Node::m_pi * 2.0;

const QColor Node::m_gold(255,215,0);
const QColor Node::m_orange(255,102,0);

Node::Node(GraphWidget *parent) :
    m_graph(parent),
    m_isActive(false),
    m_number(-1),
    m_hasBorder(false),
    m_numberIsSpecial(false),
    m_color(m_gold),
    m_textColor(0,0,0)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setDefaultTextColor(QColor(0,0,0));
    setZValue(2);
}

Node::~Node()
{
    // dtor of Edge will call removeEdgeFromList on booth nodes.
    foreach (EdgeElement element, m_edgeList) delete element.edge;
}

void Node::moveNode(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void Node::addEdge(Edge *edge, bool startsFromThisNode)
{
    m_edgeList.push_back(EdgeElement(edge, startsFromThisNode));
    edge->adjust();
}

void Node::deleteEdge(Node *otherEnd)
{
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        if ((it->edge->sourceNode() == otherEnd &&
             it->edge->destNode() == this)
         || (it->edge->sourceNode() == this &&
             it->edge->destNode() == otherEnd))
        {
            delete it->edge;
            return;
        }
    }
}

void Node::removeEdgeFromList(Edge *edge)
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

//void Node::adjustEdges()
//{
//    foreach (EdgeElement element, m_edgeList) element.edge->adjust();
//}

void Node::setBorder(const bool &hasBorder)
{
   m_hasBorder = hasBorder;
   update();
}

/** @note Used to have some disorder here, how does an active node looks like?
* Let is have border but the color shall not change, shadow is messy too.
*/

void Node::setActive(const bool &active)
{
    m_isActive = active;
    setBorder(active);
    update();
}

void Node::setEditable(const bool &editable)
{
    if (!editable)
    {
        setTextInteractionFlags(Qt::NoTextInteraction);
        return;
    }

    setTextInteractionFlags(Qt::TextEditable);
    QTextCursor c = textCursor();
    c.setPosition(c.document()->toPlainText().length());
    setTextCursor(c);
}

void Node::setColor(const QColor &color)
{
    m_color = color;
    update();
}

void Node::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void Node::setScale(const qreal &factor,const QRectF &sceneRect)
{
    if (factor * scale() < 0.4 ||
        factor * scale() > 4 )
        return;

    if (!sceneRect.contains(pos() +
                            boundingRect().bottomRight() * scale() * factor))
        return;

    prepareGeometryChange();

    QGraphicsTextItem::setScale(factor * scale());
    foreach(EdgeElement element, m_edgeList)
    {
        if (!element.startsFromThisNode)
            element.edge->setWidth(element.edge->width() * factor );

        element.edge->adjust();
    }
}

void Node::showNumber(const int &number,
                      const bool& show,
                      const bool &numberIsSpecial)
{
    m_number = show ? number : -1;
    m_numberIsSpecial = numberIsSpecial;
    update();
}

void Node::insertPicture(const QString &picture, const int &pos)
{
    QTextCursor c = textCursor();

    if (pos)
    {
        c.setPosition(pos);
    }

    c.insertHtml(QString("<img src=").append(picture).
                 append(" width=15 height=15></img>"));

    m_graph->contentChanged();
    foreach (EdgeElement element, m_edgeList) element.edge->adjust();
}

double Node::calculateBiggestAngle()
{
    if (m_edgeList.empty())
        return Node::m_oneAndHalfPi;

    if (m_edgeList.size()==1)
        return m_edgeList.first().startsFromThisNode ?
                    Node::m_pi - m_edgeList.first().edge->angle() :
                    Node::m_twoPi - m_edgeList.first().edge->angle();

    QList<double> tmp;
    for(QList<EdgeElement>::iterator it = m_edgeList.begin();
        it != m_edgeList.end(); it++)
    {
        tmp.push_back(it->startsFromThisNode ?
                 it->edge->angle() :
                 doubleModulo(Node::m_pi + it->edge->angle(), Node::m_twoPi));
    }
    qSort(tmp.begin(), tmp.end());

    double prev(tmp.first());
    double max_prev(tmp.last());
    double max(Node::m_twoPi - tmp.last() + tmp.first());

    for(QList<double>::const_iterator it = ++tmp.begin(); it!=tmp.end(); it++)
    {
        if (*it - prev > max )
        {
            max = *it - prev;
            max_prev = prev;
        }
        prev = *it;
    }

    return Node::m_twoPi - doubleModulo(max_prev + max / 2, Node::m_twoPi);
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

        // not cursor movement: editing
        QGraphicsTextItem::keyPressEvent(event);
        m_graph->contentChanged();
        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
    }

    ///@note leaving editing mode is done with esc, handled by graphwidget
}

bool Node::isConnected(const Node *node) const
{
    foreach (EdgeElement element, m_edgeList)
        if (element.edge->sourceNode() == node ||
                element.edge->destNode() == node)
            return true;

    return false;
}

QPointF Node::intersection(const QLineF &line, const bool &reverse) const
{

    /// @note What a shame, the following does not work,
    /// doing it with brute (unaccurate) force

    //    QPainterPath shape;
    //    shape.addRoundedRect(sceneBoundingRect(), 20.0, 15.0);

    //    QPainterPath l;
    //    l.moveTo(sceneBoundingRect().center());
    //    l.lineTo(line.p2());

    //    return shape.intersected(l).pointAtPercent(0.5);


    QPainterPath path;
    path.addRoundedRect(sceneBoundingRect(), 28.0, 28.0);

    if (reverse)
    {
        for (qreal t = 1; t!=0; t-=0.01)
            if (!path.contains(line.pointAt(t)))
                return line.pointAt(t);
    }
    else
    {
        for (qreal t = 0; t!=1; t+=0.01)
            if (!path.contains(line.pointAt(t)))
                return line.pointAt(t);
    }

    return QPointF(0,0);
}

QList<Edge *> Node::edgesFrom(const bool &excludeSecondaries) const
{
    QList<Edge *> list;

    foreach(EdgeElement element, m_edgeList)
        if (element.startsFromThisNode &&
                (!element.edge->secondary() || !excludeSecondaries))
            list.push_back(element.edge);

    return list;
}

QList<Edge *> Node::edgesToThis(const bool &excludeSecondaries) const
{
    QList<Edge *> list;

    foreach(EdgeElement element, m_edgeList)
        if (!element.startsFromThisNode &&
                (!element.edge->secondary() || !excludeSecondaries))
            list.push_back(element.edge);

    return list;
}

Edge * Node::edgeTo(const Node *node) const
{
    foreach(EdgeElement element, m_edgeList)
        if ((element.edge->sourceNode() == node  ||
             element.edge->destNode() == node))
            return element.edge;

    return 0;
}


QList<Node *> Node::subtree() const
{
    /** @note QList crashes if modified while traversal,
      * QMutableListIterator lacks push_back
      */

    std::list<Node *> list;
    list.push_back(const_cast<Node *>(this));

    // inorder
    for(std::list<Node *>::const_iterator it = list.begin();
        it != list.end();
        it++)
    {
        QList<Edge *> edges = (*it)->edgesFrom();
        if (!edges.empty())
        {
            foreach(Edge *edge, edges)
            {
                if (!edge->secondary())
                {
                    list.push_back( edge->destNode() != this ?
                                        edge->destNode():
                                        edge->sourceNode());
                }
            }
        }
    }
    return QList<Node *>::fromStdList(list);
}

void Node::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *w)
{
    // draw background in hint mode. num == -1 : not in hint mode
    // if m_numberIsSpecial (can be selected with enter) bg is green, not yellow
    if (m_number != -1)
    {
        painter->setPen(Qt::transparent);
        painter->setBrush(m_numberIsSpecial ? Qt::green : Qt::yellow);

        painter->drawRoundedRect(boundingRect(), 20.0, 15.0);
    }
    else
    {
        m_hasBorder ?
            painter->setPen(QPen(QBrush(Qt::black), 1)) : // border is scaled
            painter->setPen(Qt::transparent);

        painter->setBrush(m_color);
        painter->drawRoundedRect(boundingRect(), 20.0, 15.0);

    }
    painter->setBrush(Qt::NoBrush);

    // the text itself
    setDefaultTextColor(m_textColor);
    QGraphicsTextItem::paint(painter, option, w);


    // print num to topleft corner in hint mode.
    if (m_number != -1)
    {
        painter->setPen(Qt::white);
        painter->setBackground(Qt::red);
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->drawText(boundingRect().topLeft()+QPointF(0,11),
                          QString("%1").arg(m_number));
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

            // the fence is reduced with the size of the node
            QRectF rect (scene()->sceneRect().topLeft(),
                         scene()->sceneRect().bottomRight() -
                         boundingRect().bottomRight() * scale() );

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

        m_graph->contentChanged();
        foreach (EdgeElement element, m_edgeList) element.edge->adjust();
        break;

    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_graph->nodeSelected(this);

    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_graph->editNode();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_graph->nodeMoved(event);
}

QPainterPath Node::shape () const
{
    QPainterPath path;
    path.addRoundedRect(boundingRect(), 20.0, 15.0);
    return path;
}

void Node::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;

    Q_UNUSED(event);
    setEditable(false);
    m_graph->nodeLostFocus();
}

double Node::doubleModulo(const double &devided, const double &devisor)
{
    return devided - static_cast<double>(devisor * static_cast<int>(devided
                                                                  / devisor));
}
