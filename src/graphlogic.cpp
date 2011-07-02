#include "include/graphwidget.h"

#include <QtXml>
#include <QColorDialog>

GraphLogic::GraphLogic(GraphWidget *parent)
    : QObject(parent)
    , m_graphWidget(parent)
    , m_activeNode(0)
    , m_showingNodeNumbers(false)
    , m_hintNumber("")
    , m_hintNode(0)
    , m_editingNode(false)
    , m_edgeAdding(false)
    , m_edgeDeleting(false)
{

}

void GraphLogic::addFirstNode()
{
    Node *node = nodeFactory();
    node->setHtml(
                QString("<img src=:/qtmindmap.svg width=50 height=50></img>"));

    m_activeNode = m_nodeList.first();
    m_activeNode->setBorder();
}

void GraphLogic::removeAllNodes()
{
    foreach(Node *node, m_nodeList)
        delete node;

    m_nodeList.clear();
    m_activeNode = 0;
    m_hintNode = 0;
}

void GraphLogic::setActiveNode(Node *node)
{
    if (m_activeNode!=0)
        m_activeNode->setBorder(false);

    m_activeNode = node;
    if (m_activeNode)
        m_activeNode->setBorder();
}


bool GraphLogic::readContentFromXmlFile(const QString &fileName)
{
    // open & parse XML file
    QDomDocument doc("QtMindMap");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit notification(tr("Couldn't read file."));
        return false;
    }

    if (!doc.setContent(&file))
    {
        emit notification(tr("Couldn't parse XML file."));
        file.close();
        return false;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    // add nodes
    QDomNodeList nodes = docElem.childNodes().item(0).childNodes();
    for (unsigned int i = 0; i < nodes.length(); i++)
    {
        QDomElement e = nodes.item(i).toElement();
        if(!e.isNull())
        {
            Node *node = nodeFactory();
            node->setHtml(e.attribute("htmlContent"));
            node->setPos(e.attribute("x").toFloat(),
                         e.attribute("y").toFloat());
            node->setScale(e.attribute("scale").toFloat(),
                           m_graphWidget->sceneRect());
            node->setColor(QColor(e.attribute("bg_red").toFloat(),
                                  e.attribute("bg_green").toFloat(),
                                  e.attribute("bg_blue").toFloat()));
            node->setTextColor(QColor(e.attribute("text_red").toFloat(),
                                      e.attribute("text_green").toFloat(),
                                      e.attribute("text_blue").toFloat()));
        }
    }

    // add edges
    QDomNodeList edges = docElem.childNodes().item(1).childNodes();
    for (unsigned int i = 0; i < edges.length(); i++)
    {
        QDomElement e = edges.item(i).toElement();
        if(!e.isNull())
        {
            Node *source = m_nodeList[e.attribute("source").toInt()];
            Node *destination = m_nodeList[e.attribute("destination").toInt()];

            Edge *edge = new Edge(source, destination);
            source->addEdge(edge, true);
            destination->addEdge(edge, false);

            edge->setColor(QColor(e.attribute("red").toFloat(),
                                  e.attribute("green").toFloat(),
                                  e.attribute("blue").toFloat()));
            edge->setWidth(e.attribute("width").toFloat());
            edge->setSecondary(e.attribute("secondary").toInt() );

            m_graphWidget->scene()->addItem(edge);
        }
    }

    // test the first node the active one
    m_activeNode = m_nodeList.first();
    m_activeNode->setBorder();
    m_activeNode->setFocus();

    m_graphWidget->show();
    return true;
}

void GraphLogic::writeContentToXmlFile(const QString &fileName)
{
    // create XML doc object
    QDomDocument doc("QtMindMap");

    QDomElement root = doc.createElement("qtmindmap");
    doc.appendChild( root );

    // nodes
    QDomElement nodes_root = doc.createElement("nodes");
    root.appendChild(nodes_root);
    foreach(Node *node, m_nodeList)
    {
        QDomElement cn = doc.createElement("node");

        // no need to store ID: parsing order is preorder.
        // cn.setAttribute( "id", QString::number(m_nodeList.indexOf(node)));
        cn.setAttribute( "x", QString::number(node->pos().x()));
        cn.setAttribute( "y", QString::number(node->pos().y()));
        cn.setAttribute( "htmlContent", node->toHtml());
        cn.setAttribute( "scale", QString::number(node->scale()));
        cn.setAttribute( "bg_red", QString::number(node->color().red()));
        cn.setAttribute( "bg_green", QString::number(node->color().green()));
        cn.setAttribute( "bg_blue", QString::number(node->color().blue()));
        cn.setAttribute( "text_red", QString::number(node->textColor().red()));
        cn.setAttribute( "text_green",
                        QString::number(node->textColor().green()));
        cn.setAttribute( "text_blue",
                        QString::number(node->textColor().blue()));
        nodes_root.appendChild(cn);
    }

    //edges
    QDomElement edges_root = doc.createElement("edges");
    root.appendChild(edges_root);
    foreach(Edge *edge, allEdges())
    {
        QDomElement cn = doc.createElement("edge");
        cn.setAttribute( "source",
                      QString::number(m_nodeList.indexOf(edge->sourceNode())));
        cn.setAttribute( "destination",
                      QString::number(m_nodeList.indexOf(edge->destNode())));
        cn.setAttribute( "red", QString::number(edge->color().red()));
        cn.setAttribute( "green", QString::number(edge->color().green()));
        cn.setAttribute( "blue", QString::number(edge->color().blue()));
        cn.setAttribute( "width", QString::number(edge->width()));
        cn.setAttribute( "secondary", QString::number(edge->secondary()));

        edges_root.appendChild(cn);
    }

    // write XML doc object to file
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        emit notification(tr("Couldn't open file to write."));
        return;
    }
    QTextStream ts( &file );
    ts << doc.toString();
    file.close();

    // show a statusBar message to the user
    emit notification(tr("Saved."));
}

void GraphLogic::writeContentToPngFile(const QString &fileName)
{
    QImage img(m_graphWidget->scene()->sceneRect().width(),
               m_graphWidget->scene()->sceneRect().height(),
               QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);

    painter.setRenderHint(QPainter::Antialiasing);

    // Strange that I have to set this, and scene->render() does not do this
    m_graphWidget->scene()->setBackgroundBrush(GraphWidget::m_paper);

    m_graphWidget->scene()->render(&painter);
    painter.setBackground(GraphWidget::m_paper);
    painter.end();

    img.save(fileName);

    // show a statusBar message to the user
    emit notification(tr("MindMap exported as ") + fileName);
}

bool GraphLogic::editing() const
{
    return m_editingNode;
}

void GraphLogic::passKey(QKeyEvent *event)
{
    m_activeNode->keyPressEvent(event);
}

void GraphLogic::insertNode()
{
    nodeLostFocus();

    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // get the biggest angle between the edges of the Node.
    double angle(m_activeNode->calculateBiggestAngle());

    // let the distance between the current and new Node be 100 pixels
    qreal length(100);

    QPointF pos(length * cos(angle), length * sin(angle));

    QPointF newPos(m_activeNode->sceneBoundingRect().center() +
                   pos - Node::newNodeCenter);
    QRectF rect (m_graphWidget->scene()->sceneRect().topLeft(),
                 m_graphWidget->scene()->sceneRect().bottomRight()
                 - Node::newNodeBottomRigth);

    if (!rect.contains(newPos))
    {
        emit notification(tr("New node would be placed outside of the scene"));
        return;
    }

    // add a new node which inherits the color and textColor
    Node *node = nodeFactory();
    node->setColor(m_activeNode->color());
    node->setTextColor(m_activeNode->textColor());
    node->setHtml(QString(""));
    node->setPos(newPos);

    addEdge(m_activeNode, node);

    // set it the active Node and editable, so the user can edit it at once
    setActiveNode(node);
    nodeEdited();

    emit contentChanged();

    // it we are in hint mode, the numbers shall be re-calculated
    if (m_showingNodeNumbers)
        showNodeNumbers();
}

void GraphLogic::removeNode(const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (m_activeNode == m_nodeList.first())
    {
        emit notification(tr("Base node cannot be deleted."));
        return;
    }

    // remove just the active Node or it's subtree too?
    QList <Node *> nodeList;
    if (subtree)
    {
        nodeList = m_activeNode->subtree();
    }
    else
    {
        nodeList.push_back(m_activeNode);
    }

    foreach(Node *node, nodeList)
    {
        if (m_hintNode==node)
            m_hintNode=0;

        m_nodeList.removeAll(node);
        delete node;
    }

    m_activeNode = 0;
    emit contentChanged();

    // it we are in hint mode, the numbers shall be re-calculated
    if (m_showingNodeNumbers)
        showNodeNumbers();
}

void GraphLogic::scaleUp(const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (subtree)
    {
        QList <Node *> nodeList = m_activeNode->subtree();
        foreach(Node *node, nodeList)
            node->setScale(qreal(1.2), m_graphWidget->sceneRect());
    }
    else
    {
        m_activeNode->setScale(qreal(1.2),m_graphWidget->sceneRect());
    }
}

void GraphLogic::scaleDown(const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (subtree)
    {
        QList <Node *> nodeList = m_activeNode->subtree();
        foreach(Node *node, nodeList)
            node->setScale(qreal(1 / 1.2),m_graphWidget->sceneRect());
    }
    else
    {
        m_activeNode->setScale(qreal(1 / 1.2),m_graphWidget->sceneRect());
    }
}

void GraphLogic::setNodeColor(const QColor &color, const bool &subtree)
{
    QList <Node *> nodeList;
    if (subtree)
    {
        nodeList = m_activeNode->subtree();
    }
    else
    {
        nodeList.push_back(m_activeNode);
    }

    foreach(Node *node, nodeList)
    {
        node->setColor(color);
        foreach (Edge * edge, node->edgesToThis(false))
            edge->setColor(color);
    }
}

void GraphLogic::setNodeTextColor(const QColor &color, const bool &subtree)
{
    QList <Node *> nodeList;
    if (subtree)
    {
        nodeList = m_activeNode->subtree();
    }
    else
    {
        nodeList.push_back(m_activeNode);
    }

    foreach(Node *node, nodeList)
        node->setTextColor(color);
}

void GraphLogic::addEdge()
{
    emit notification(tr("Add edge: select destination node."));
    m_edgeAdding = true;
}

void GraphLogic::removeEdge()
{
    emit notification(tr("Delete edge: select other end-node."));
    m_edgeDeleting = true;
}

void GraphLogic::hintMode()
{
    // vimperator-style node selection with keys.
    // show or hide the numbers if we enter/leave this mode.
    m_showingNodeNumbers = !m_showingNodeNumbers;
    if (!m_showingNodeNumbers)
    {
        showingAllNodeNumbers(false);
        return;
    }

    m_hintNumber.clear();
    showNodeNumbers();
}


void GraphLogic::insertPicture(const QString &picture)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    m_activeNode->insertPicture(picture);
}

void GraphLogic::move(const int &x, const int &y, const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (subtree)
    {
        QList <Node *> nodeList = m_activeNode->subtree();
        foreach(Node *node, nodeList)
            node->moveBy(x, y);

        emit contentChanged();
    }
    else // Move just the active Node.
    {
        m_activeNode->moveBy(x, y);
        emit contentChanged();
    }
}

void GraphLogic::appendNumber(const int &num)
{
    if (!m_showingNodeNumbers)
        return;

    m_hintNumber.append(QString::number(num));

    showingAllNodeNumbers(false);
    showingNodeNumbersBeginWithNumber(m_hintNumber.toInt(), true);
}

void GraphLogic::delNumber()
{
    if (!m_showingNodeNumbers && m_hintNumber.isEmpty())
        return;

    m_hintNumber.remove(m_hintNumber.length()-1,1);
    showNodeNumbers();
}

void GraphLogic::applyNumber()
{
    if (m_hintNode && m_showingNodeNumbers)
        selectNode(m_hintNode);
}

void GraphLogic::nodeColor(const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // popup a color selector dialogm def color is the curr one.
    QColorDialog dialog(m_graphWidget);
    dialog.setWindowTitle(tr("Select node color"));
    dialog.setCurrentColor(m_activeNode->color());
    if (!dialog.exec())
        return;

    setNodeColor(dialog.selectedColor(), subtree);
}

void GraphLogic::nodeTextColor(const bool &subtree)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    // popup a color selector dialogm def color is the curr one.
    QColorDialog dialog(m_graphWidget);
    dialog.setWindowTitle(tr("Select text color"));
    dialog.setCurrentColor(m_activeNode->textColor());
    if (!dialog.exec())
        return;

    setNodeTextColor(dialog.selectedColor(), subtree);
}

void GraphLogic::nodeChanged()
{
    emit contentChanged();
}

void GraphLogic::nodeSelected()
{
    // if node == 0 then nodeSelected invoked after a signal from a Node
    selectNode(dynamic_cast<Node*>(QObject::sender()));
}

void GraphLogic::nodeEdited(QKeyEvent *event)
{
    Q_UNUSED(event)
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    m_editingNode = true;
    m_activeNode->setEditable();
    m_graphWidget->scene()->setFocusItem(m_activeNode);
}

void GraphLogic::nodeMoved(QGraphicsSceneMouseEvent *event)
{
    // move just the active Node, or it's subtree too?
    QList <Node *> nodeList;
    if (event->modifiers() & Qt::ControlModifier &&
        event->modifiers() & Qt::ShiftModifier)
    {
        nodeList = m_activeNode->subtree();
    }
    else
    {
        nodeList.push_back(m_activeNode);
    }

    foreach(Node *node, nodeList)
        node->setPos(node->pos() + event->scenePos() - event->lastScenePos());
}


void GraphLogic::nodeLostFocus()
{
    if (m_editingNode)
    {
        m_editingNode = false;
        if (m_activeNode)
        {
            m_activeNode->setEditable(false);
            m_activeNode->update();
        }
        return;
    }

    if (m_edgeAdding)
    {
        m_edgeAdding = false;
        emit notification(tr("Edge adding cancelled."));
        return;
    }

    if (m_edgeDeleting)
    {
        m_edgeDeleting = false;
        emit notification(tr("Edge deleting cancelled."));
        return;
    }

    if(m_showingNodeNumbers)
    {
        m_hintNumber.clear();
        showingAllNodeNumbers(false);
        m_showingNodeNumbers = false;
        return;
    }
}

Node * GraphLogic::nodeFactory()
{
    Node *node = new Node();

    connect(node, SIGNAL(nodeChanged()), this, SLOT(nodeChanged()));
    connect(node, SIGNAL(nodeSelected()), this, SLOT(nodeSelected()));
    connect(node, SIGNAL(nodeEdited()), this, SLOT(nodeEdited()));
    connect(node, SIGNAL(nodeMoved(QGraphicsSceneMouseEvent*)),
            this, SLOT(nodeMoved(QGraphicsSceneMouseEvent*)));
    connect(node, SIGNAL(nodeLostFocus()), this, SLOT(nodeLostFocus()));

    m_graphWidget->scene()->addItem(node);
    m_nodeList.append(node);

    return node;
}

void GraphLogic::selectNode(Node *node)
{
    // leave hint mode
    showingAllNodeNumbers(false);
    m_showingNodeNumbers = false;

    if (m_edgeAdding)
    {
        addEdge(m_activeNode, node);
        m_edgeAdding = false;
    }
    else if (m_edgeDeleting)
    {
        removeEdge(m_activeNode, node);
        m_edgeDeleting = false;
    }
    else
    {
        setActiveNode(node);
    }
}

QList<Edge *> GraphLogic::allEdges() const
{
    QList<Edge *> list;

    // GraphWidget has a list of Nodes only.
    // Each Node maintains a list of it's own Edges.
    // We iterate through the list of Nodes and call Node::edgesFrom() on them.
    // edgesFrom(exludeSecundaries=false) return a list of edges (including
    // secondary edges) which starts from this Node.
    foreach(Node * node, m_nodeList)
        list.append(node->edgesFrom(false));

    return list;
}


void GraphLogic::addEdge(Node *source, Node *destination)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (destination == m_nodeList.first())
    {
        setActiveNode(destination);
        emit notification(
                    tr("Root element cannot be an edge target."));
        return;
    }

    if (source->isConnected(destination))
    {
        setActiveNode(destination);
        emit notification(
                    tr("There is already an edge between these two nodes."));
    }
    else
    {
        // aviod the graph beeing acyclic. (ok, Nodes having multiple parents)
        bool sec(false);
        if (!destination->edgesToThis().empty())
        {
            emit notification(
                     tr("The graph is acyclic, edge added as secondary edge."));
            sec = true;
        }
        Edge *edge = new Edge(source, destination);
        source->addEdge(edge, true);
        destination->addEdge(edge, false);

        edge->setColor(destination->color());
        edge->setWidth(destination->scale()*2 + 1);

        // The Edge is secondary, because the Node already has a parent
        // (it is already a destination of another Edge)
        edge->setSecondary(sec);
        m_graphWidget->scene()->addItem(edge);

        setActiveNode(destination);
        emit contentChanged();
    }
}

void GraphLogic::removeEdge(Node *source, Node *destination)
{
    if (!m_activeNode)
    {
        emit notification(tr("No active node."));
        return;
    }

    if (!source->isConnected(destination))
    {
        setActiveNode(destination);
        emit notification(tr("There no edge between these two nodes."));
    }
    else
    {
        source->deleteEdge(destination);
        setActiveNode(destination);
        emit contentChanged();
    }
}

// re-draw numbers
void GraphLogic::showNodeNumbers()
{
    if (m_hintNumber.isEmpty())
     {
         showingAllNodeNumbers(true);
         m_nodeList.first()->showNumber(0,true,true);
         m_hintNode = m_nodeList.first();
     }
     else
     {
         showingAllNodeNumbers(false);
         showingNodeNumbersBeginWithNumber(m_hintNumber.toInt(), true);
     }
}

// show/hide numbers on all nodes
void GraphLogic::showingAllNodeNumbers(const bool &show)
{
    int i(0);
    for (QList<Node *>::const_iterator it = m_nodeList.begin();
         it != m_nodeList.end(); it++,
         i++)
    {
        dynamic_cast<Node*>(*it)->showNumber(i,show);
    }
}

// show nodes numbers where number begins with 'prefix'
void GraphLogic::showingNodeNumbersBeginWithNumber(const int &prefix,
                                                    const bool &show)
{
    int i(0);
    int hit(0);
    for (QList<Node *>::const_iterator it = m_nodeList.begin();
         it != m_nodeList.end(); it++, i++)
    {
        // if nodenumber == 'prefix' the node is selected
        if (i == prefix)
        {
            hit++;
            dynamic_cast<Node*>(*it)->showNumber(i,show,true);
            m_hintNode = dynamic_cast<Node*>(*it);
            continue;
        }

        // if 'i' starts with 'prefix'
        if ((QString::number(i)).startsWith(QString::number(prefix)))
        {
            hit++;
            dynamic_cast<Node*>(*it)->showNumber(i,show);
        }
    }
    if (hit==1)
    {
        selectNode(m_hintNode);
    }
    else if (hit == 0)
    {
        m_showingNodeNumbers = false;
        showingAllNodeNumbers(false);
    }
}
