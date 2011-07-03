#include "include/commands.h"

#include <QDebug>

#include <math.h>

InsertNodeCommand::InsertNodeCommand(GraphLogic *graphLogic)
    : m_graphLogic(graphLogic)
    , m_node(0)
    , m_activeNode(m_graphLogic->m_activeNode)
{
    if (!m_activeNode)
        throw NoActiveNodeException();

    setText(QObject::tr("New node added to ").append(
            m_activeNode == m_graphLogic->m_nodeList.first() ?
              QObject::tr("Base node") :
              QString("\"").append(m_activeNode->toPlainText().append("\""))));

    m_graphLogic->nodeLostFocus();

    // get the biggest angle between the edges of the Node.
    double angle(m_activeNode->calculateBiggestAngle());

    // let the distance between the current and new Node be 100 pixels
    qreal length(100);

    m_pos = m_activeNode->sceneBoundingRect().center() +
            QPointF(length * cos(angle), length * sin(angle)) -
            Node::newNodeCenter;

    QRectF rect (m_graphLogic->m_graphWidget->scene()->sceneRect().topLeft(),
                 m_graphLogic->m_graphWidget->scene()->sceneRect().bottomRight()
                 - Node::newNodeBottomRigth);

    if (!rect.contains(m_pos))
        throw CannotPlaceNewNodeException();

    // add a new node which inherits the color and textColor
    m_node = m_graphLogic->nodeFactory();
    m_node->setColor(m_activeNode->color());
    m_node->setTextColor(m_activeNode->textColor());
    m_node->setHtml(QString(""));
}

void InsertNodeCommand::undo()
{
    m_graphLogic->m_nodeList.removeAll(m_node);
    m_graphLogic->m_graphWidget->scene()->removeItem(m_node);

    m_node->removeEdges();

    m_graphLogic->setActiveNode(m_activeNode);

    emit  m_graphLogic->contentChanged(false);
}

void InsertNodeCommand::redo()
{
    m_graphLogic->m_graphWidget->scene()->addItem(m_node);
    m_graphLogic->m_nodeList.append(m_node);

    m_node->setPos(m_pos);

    m_graphLogic->addEdge(m_activeNode, m_node);

    m_graphLogic->setActiveNode(m_node);

    if (m_graphLogic->m_graphWidget->hasFocus())
        m_graphLogic->nodeEdited();

    emit m_graphLogic->contentChanged();

    // it we are in hint mode, the numbers shall be re-calculated
    if (m_graphLogic->m_showingNodeNumbers)
        m_graphLogic->showNodeNumbers();
}
