#include "include/commands.h"

#include <QDebug>
#include <QApplication>

#include <math.h>



const char* NoActiveNodeException::what() const throw()
{
    return QObject::tr("No active node.").toStdString().c_str();
}

const char* CannotPlaceNewNodeException::what() const throw()
{
    return QObject::tr("New node would be placed outside of the scene.").
            toStdString().c_str();
}

const char* CannotDeleteBaseNodeException::what() const throw()
{
    return QObject::tr("Base node cannot be deleted.").toStdString().c_str();
}

const char* BaseNodeCannotBeEdgeTargetException::what() const throw()
{
    return QObject::tr("Base node cannot be a target.").toStdString().c_str();
}

const char* EdgeExistsBetweenNodesException::what() const throw()
{
    return QObject::tr("There is already an edge between these two nodes.").
            toStdString().c_str();
}


InsertNodeCommand::InsertNodeCommand(GraphLogic *graphLogic)
    : m_graphLogic(graphLogic)
    , m_node(0)
    , m_activeNode(m_graphLogic->m_activeNode)
    , m_edge(0)
{
    if (!m_activeNode)
        throw NoActiveNodeException();

    setText(QObject::tr("Node added to ").append(
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

    m_edge = new Edge(m_activeNode, m_node);
    m_edge->setColor(m_node->color());
    m_edge->setWidth(m_node->scale()*2 + 1);
    m_edge->setSecondary(false);
}

void InsertNodeCommand::undo()
{
    m_graphLogic->m_nodeList.removeAll(m_node);
    m_graphLogic->m_graphWidget->scene()->removeItem(m_node);

//    m_node->deleteEdges();
    m_edge->sourceNode()->removeEdge(m_edge);
    m_edge->destNode()->removeEdge(m_edge);
    m_graphLogic->m_graphWidget->scene()->removeItem(m_edge);

    m_graphLogic->setActiveNode(m_activeNode);

    emit  m_graphLogic->contentChanged(false);
}

void InsertNodeCommand::redo()
{
    m_graphLogic->m_graphWidget->scene()->addItem(m_node);
    m_graphLogic->m_nodeList.append(m_node);

    m_node->setPos(m_pos);

//    m_graphLogic->addEdge(m_activeNode, m_node);
    m_edge->sourceNode()->addEdge(m_edge,true);
    m_edge->destNode()->addEdge(m_edge,false);
    m_graphLogic->m_graphWidget->scene()->addItem(m_edge);

    m_graphLogic->setActiveNode(m_node);

    if (m_graphLogic->m_graphWidget->hasFocus())
        m_graphLogic->nodeEdited();

    emit m_graphLogic->contentChanged();

    // it we are in hint mode, the numbers shall be re-calculated
    if (m_graphLogic->m_showingNodeNumbers)
        m_graphLogic->showNodeNumbers();
}

RemoveNodeCommand::RemoveNodeCommand(GraphLogic *graphLogic)
    : m_graphLogic(graphLogic)
    , m_activeNode(m_graphLogic->m_activeNode)
    , m_hintNode(m_graphLogic->m_hintNode)
{
    if (!m_activeNode)
        throw NoActiveNodeException();

    if (m_activeNode == m_graphLogic->m_nodeList.first())
        throw CannotDeleteBaseNodeException();

    setText(QObject::tr("Node removed \"").append(
                m_activeNode->toPlainText().append("\"")));

    // remove just the active Node or it's subtree too?
    if (QApplication::keyboardModifiers() & Qt::ControlModifier &&
        QApplication::keyboardModifiers() & Qt::ShiftModifier)
    {
        m_nodeList = m_activeNode->subtree();
        setText(text().append(QObject::tr(" with subtree")));
    }
    else
    {
        m_nodeList.push_back(m_activeNode);
    }

    foreach(Node *node, m_nodeList)
        foreach(Edge *edge, node->edges())
            if (m_edgeList.indexOf(edge) == -1)
                m_edgeList.push_back(edge);
}

void RemoveNodeCommand::undo()
{
    foreach (Node *node, m_nodeList)
    {
        m_graphLogic->m_graphWidget->scene()->addItem(node);
        m_graphLogic->m_nodeList.append(node);
    }

    foreach (Edge *edge, m_edgeList)
    {
        edge->sourceNode()->addEdge(edge,true);
        edge->destNode()->addEdge(edge,false);
        m_graphLogic->m_graphWidget->scene()->addItem(edge);
    }

    emit m_graphLogic->contentChanged(false);
    m_graphLogic->m_activeNode = m_activeNode;
    m_graphLogic->m_hintNode = m_hintNode;


    // it we are in hint mode, the numbers shall be re-calculated
    if (m_graphLogic->m_showingNodeNumbers)
        m_graphLogic->showNodeNumbers();
}

void RemoveNodeCommand::redo()
{
    foreach(Node *node, m_nodeList)
    {
        if (m_graphLogic->m_hintNode==node)
            m_graphLogic->m_hintNode=0;

        m_graphLogic->m_nodeList.removeAll(node);
        m_graphLogic->m_graphWidget->scene()->removeItem(node);
    }

    foreach(Edge *edge, m_edgeList)
    {
        edge->sourceNode()->removeEdge(edge);
        edge->destNode()->removeEdge(edge);
        m_graphLogic->m_graphWidget->scene()->removeItem(edge);
    }

    m_graphLogic->m_activeNode = 0;

    emit m_graphLogic->contentChanged();

    // it we are in hint mode, the numbers shall be re-calculated
    if (m_graphLogic->m_showingNodeNumbers)
        m_graphLogic->showNodeNumbers();
}

AddEdgeCommand::AddEdgeCommand(GraphLogic *graphLogic, Node *source, Node *destinaion)
    : m_graphLogic(graphLogic)
    , m_activeNode(m_graphLogic->m_activeNode)
    , m_source(source)
    , m_destination(destinaion)
{
    if (!m_activeNode)
        throw NoActiveNodeException();

    if (m_destination == m_graphLogic->m_nodeList.first())
        throw BaseNodeCannotBeEdgeTargetException();


    if (m_source->isConnected(m_destination))
        throw EdgeExistsBetweenNodesException();

    setText(QObject::tr("Edge added between \"").append(
                m_source->toPlainText()).append(
                QObject::tr("\" and \"").append(
                m_destination->toPlainText()).append("\"")));


    // aviod the graph beeing acyclic. (ok, Nodes having multiple parents)
    bool sec(false);
    if (!m_destination->edgesToThis().empty())
    {
        emit m_graphLogic->notification(
           QObject::tr("The graph is acyclic, edge added as secondary edge."));
        sec = true;
    }

    m_edge = new Edge(m_source, m_destination);
    m_edge->setColor(m_destination->color());
    m_edge->setWidth(m_destination->scale()*2 + 1);

    // The Edge is secondary, because the Node already has a parent
    // (it is already a destination of another Edge)
    m_edge->setSecondary(sec);
}

void AddEdgeCommand::undo()
{
    m_source->removeEdge(m_edge);
    m_destination->removeEdge(m_edge);
    m_graphLogic->m_graphWidget->scene()->removeItem(m_edge);

    m_graphLogic->setActiveNode(m_activeNode);

    emit  m_graphLogic->contentChanged(false);
}

void AddEdgeCommand::redo()
{
    m_source->addEdge(m_edge, true);
    m_destination->addEdge(m_edge, false);

    m_graphLogic->m_graphWidget->scene()->addItem(m_edge);

    m_graphLogic->setActiveNode(m_destination);
    emit m_graphLogic->contentChanged();
}

RemoveEdgeCommand::RemoveEdgeCommand(GraphLogic *graphLogic, Node *source, Node *destinaion)
    : m_graphLogic(graphLogic)
    , m_activeNode(m_graphLogic->m_activeNode)
    , m_source(source)
    , m_destination(destinaion)
    , m_edge(source->edgeTo(destinaion))
{
    setText(QObject::tr("Edge releted between \"").append(
            m_source->toPlainText()).append(
            QObject::tr("\" and \"").append(
            m_destination->toPlainText()).append("\"")));
}

void RemoveEdgeCommand::undo()
{
    m_source->addEdge(m_edge, true);
    m_destination->addEdge(m_edge, false);

    m_graphLogic->m_graphWidget->scene()->addItem(m_edge);

    m_graphLogic->setActiveNode(m_destination);
    emit m_graphLogic->contentChanged(false);
}

void RemoveEdgeCommand::redo()
{
    m_source->removeEdge(m_edge);
    m_destination->removeEdge(m_edge);
    m_graphLogic->m_graphWidget->scene()->removeItem(m_edge);

    m_graphLogic->setActiveNode(m_activeNode);

    emit  m_graphLogic->contentChanged();
}
