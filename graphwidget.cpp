#include "graphwidget.h"

#include <QDebug>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QtXml>

#include "node.h"
#include "edge.h"
#include "math.h"
#include "mainwindow.h"


GraphWidget::GraphWidget(MainWindow *parent) :
    QGraphicsView(parent),
    m_parent(parent),
    m_activeNode(0),
    m_showingNodeNumbers(false),
    m_hintNode(0),
    m_editingNode(false),
    m_edgeAdding(false),
    m_edgeDeleting(false),
    m_contentChanged(false)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(-400, -400, 800, 800);
    setScene(m_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);

//    Node *node1 = new Node(this);
//    node1->setHtml(QString("<img src=:/heart.svg width=40 height=40></img>"));
//    m_scene->addItem(node1);
//    node1->setPos(-10, -10);
//    node1->setBorder(false);
//    m_nodeList.append(node1);

    /*
    Node *node2 = new Node(this);
    node2->setHtml(QString("work: <a href=www.hup.hu>hup.hu</a>"));
    m_scene->addItem(node2);
    node2->setPos(60, -10);
    m_nodeList.append(node2);

    Node *node3 = new Node(this);
    node3->setHtml(QString("read"));
    m_scene->addItem(node3);
    node3->setPos(-70, -10);
    m_nodeList.append(node3);

    Node *node4 = new Node(this);
    node4->setHtml(QString("pragmatic programmer"));
    m_scene->addItem(node4);
    node4->setPos(-120, -80);
    m_nodeList.append(node4);

    Node *node5 = new Node(this);
    node5->setHtml(QString("semmi kulonos"));
    m_scene->addItem(node5);
    node5->setPos(-10, 50);
    m_nodeList.append(node5);

    Node *node6 = new Node(this);
    node6->setHtml(QString("rape goats"));
    m_scene->addItem(node6);
    node6->setPos(-10, 100);
    m_nodeList.append(node6);

    Node *node7 = new Node(this);
    node7->setHtml(QString("node <h1>important</h1> shit"));
    m_scene->addItem(node7);
    node7->setPos(-200, 50);
    m_nodeList.append(node7);

    Node *node8 = new Node(this);
    node8->setHtml(QString("more than\none lins"));
    m_scene->addItem(node8);
    node8->setPos(50, -80);
    m_nodeList.append(node8);

    Node *node9 = new Node(this);
    node9->setHtml(QString("iam a <b>bald</b> and <i>italian</i> guy"));
    m_scene->addItem(node9);
    node9->setPos(90, 90);
    m_nodeList.append(node9);

    Node *node10 = new Node(this);
    node10->setHtml(QString("no joke <br> anotehr line"));
    m_scene->addItem(node10);
    node10->setPos(-160, -10);
    m_nodeList.append(node10);

    Node *node11 = new Node(this);
    node11->setHtml(QString("salalal"));
    m_scene->addItem(node11);
    node11->setPos(-120, -120);
    m_nodeList.append(node11);

    Node *node12 = new Node(this);
    node12->setHtml(QString("lalalala"));
    m_scene->addItem(node12);
    node12->setPos(170, -10);
    m_nodeList.append(node12);;
    node12->setTextInteractionFlags(Qt::TextEditable);

    m_scene->addItem(new Edge(node1, node2));
    m_scene->addItem(new Edge(node1, node3));
    m_scene->addItem(new Edge(node3, node4));
    m_scene->addItem(new Edge(node1, node5));
    m_scene->addItem(new Edge(node5, node6));
    m_scene->addItem(new Edge(node4, node11));
    m_scene->addItem(new Edge(node2, node12));
    m_scene->addItem(new Edge(node3, node10));
    m_scene->addItem(new Edge(node10, node7));
    m_scene->addItem(new Edge(node5, node7));
    m_scene->addItem(new Edge(node2, node8));
    m_scene->addItem(new Edge(node2, node9));
    */

//    m_activeNode = m_nodeList.first();
//    m_activeNode->setActive();

    //addFirstNode();

    /// @todo open file somewhere
}

void GraphWidget::newFile()
{
    removeAllNodes();

    addFirstNode();

    this->show();

    m_parent->enableCloseFile(true);
    m_parent->enableSave(false);
    m_parent->enableSaveAs(true);
    m_parent->setTitle("untitled");
}

void GraphWidget::closeFile()
{
    m_contentChanged = true;

    if (m_contentChanged)
    {

        int ret = QMessageBox::warning(
                    this,
                    tr("QtMindMap - The document has been modified"),
                    m_parent->getFileName().
                    append("\n\n").
                    append(tr("Do you want to save your changes?")),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                    QMessageBox::Save);

        switch (ret) {
        case QMessageBox::Save:
            // Save was clicked
        {
            QString fileName = QFileDialog::getSaveFileName(
                        this,
                        tr("Save File"),
                        QDir::homePath(),
                        tr("QtMindMap (*.qmm)"));

            qDebug() << fileName;

            /// @todo save content to file

            break;
        }
        case QMessageBox::Discard:
            // Don't Save was clicked
            removeAllNodes();
            this->hide();
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked
            return;
        default:
            // should never be reached
            break;
        }
    }
    else
    {
        removeAllNodes();
        this->hide();
    }

    m_parent->enableCloseFile(false);
    m_parent->enableSave(false);
    m_parent->enableSaveAs(false);
    m_parent->setTitle("");
}

void GraphWidget::saveFileAs()
{
    QFileDialog dialog(this,
                       tr("Save MindMap as"),
                       QDir::homePath(),
                       tr("QtMindMap (*.qmm)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("qmm");

    if (dialog.exec())
    {
        m_fileName = dialog.selectedFiles().first();
        saveFile();
    }
}

void GraphWidget::saveFile()
{
    QDomDocument doc("QtMindMap");

    QDomElement root = doc.createElement("qtmindmap");
    doc.appendChild( root );

    // nodes
    QDomElement nodes_root = doc.createElement("nodes");
    root.appendChild(nodes_root);
    foreach(Node *node, m_nodeList)
    {
        QDomElement cn = doc.createElement("nodes");
        cn.setAttribute( "id", QString::number(m_nodeList.indexOf(node)));
        cn.setAttribute( "x", QString::number(node->pos().x()));
        cn.setAttribute( "y", QString::number(node->pos().y()));
        cn.setAttribute( "htmlContent", node->toHtml());
        nodes_root.appendChild(cn);
    }

    //edges
    QDomElement edges_root = doc.createElement("edges");
    root.appendChild(edges_root);
    foreach(Edge *edge, edges())
    {
        QDomElement cn = doc.createElement("edge");
        cn.setAttribute( "source", QString::number(m_nodeList.indexOf(edge->sourceNode())));
        cn.setAttribute( "destination", QString::number(m_nodeList.indexOf(edge->destNode())));
        edges_root.appendChild(cn);
    }

    QFile file(m_fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("Couldn't open file to write."),
                    3000); // millisec
        return;
    }

    QTextStream ts( &file );
    ts << doc.toString();
    file.close();

    dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                tr("Saved."),
                3000); // millisec

    m_parent->enableCloseFile(true);
    m_parent->enableSave(true);
    m_parent->enableSaveAs(true);

    m_parent->setTitle(m_fileName);
}

void GraphWidget::openFile()
{
    qDebug() << __PRETTY_FUNCTION__;

    QFileDialog dialog(this,
                       tr("Open MindMap"),
                       QDir::homePath(),
                       tr("QtMindMap (*.qmm)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDefaultSuffix("qmm");

    if (dialog.exec())
    {
        m_fileName = dialog.selectedFiles().first();
        openFile(m_fileName);
    }
}

void GraphWidget::openFile(const QString &fileName)
{
    m_fileName = fileName;
    m_parent->enableCloseFile(true);
    m_parent->enableSave(true);
    m_parent->enableSaveAs(true);

    m_parent->setTitle(fileName);

    QDomDocument doc("QtMindMap");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "faszom cannot read file";
        return;
    }

    if (!doc.setContent(&file))
    {
        qDebug() << "cannot parse file";
        file.close();
        return;
    }
    file.close();

    removeAllNodes();

    QDomElement docElem = doc.documentElement();

    QDomNodeList nodes = docElem.childNodes().item(0).childNodes();
    for (unsigned int i = 0; i < nodes.length(); i++)
    {
        QDomElement e = nodes.item(i).toElement();
        if(!e.isNull())
        {
            Node *node = new Node(this);
            node->setHtml(e.attribute("htmlContent"));
            m_scene->addItem(node);
            node->setPos(e.attribute("x").toFloat(),
                         e.attribute("y").toFloat());
            m_nodeList.append(node);
        }
    }

    QDomNodeList edges = docElem.childNodes().item(1).childNodes();
    for (unsigned int i = 0; i < edges.length(); i++)
    {
        QDomElement e = edges.item(i).toElement();
        if(!e.isNull())
        {
            m_scene->addItem(new Edge(m_nodeList[e.attribute("source").toInt()],

                                      m_nodeList[e.attribute("destination").toInt()]));
        }
    }

    m_activeNode = m_nodeList.first();
    m_activeNode->setActive();

    this->show();
}


QGraphicsScene *GraphWidget::getScene()
{
    return m_scene;
}


void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    // esc leaves node editing mode
    if (event->key() == Qt::Key_Escape && m_editingNode)
    {
        m_activeNode->setEditable(false);
        m_editingNode = false;
        return;
    }
    // in node editing mode forward every key (except esc) to node
    else if (m_editingNode)
    {
        m_activeNode->keyPressEvent(event);
        return;
    }

    // certain actions need an active node
    if (!m_activeNode &&
            (event->key() == Qt::Key_Insert ||      // add new node
             event->key() == Qt::Key_F2 ||          // edit node
             event->key() == Qt::Key_Delete ||      // delete node
             event->key() == Qt::Key_A ||           // add edge
             event->key() == Qt::Key_D ||           // remove edge
             ( event->modifiers() ==  Qt::ControlModifier &&  // moving node
               ( event->key() == Qt::Key_Up ||
                 event->key() == Qt::Key_Down ||
                 event->key() == Qt::Key_Left ||
                 event->key() == Qt::Key_Right))))
    {
        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("No active node."),
                    3000); // millisec
        return;
    }

    switch (event->key())
    {

    case Qt::Key_Escape:

        if (m_edgeAdding)
        {
            m_edgeAdding = false;
            dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                        tr("Edge adding cancelled"),
                        3000); // millisec
        }
        else if (m_edgeDeleting)
        {
            m_edgeDeleting = false;
            dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                        tr("Edge deleting cancelled"),
                        3000); // millisec
        }
        else if(m_showingNodeNumbers)
        {
            m_hintNumber.clear();
            showingAllNodeNumbers(false);
            m_showingNodeNumbers = false;
        }

        break;

        // move sceve, or move node if modkey is ctrl
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        if (event->modifiers() ==  Qt::ControlModifier)
        {
            if (event->key() == Qt::Key_Up) m_activeNode->moveBy(0, -20);
            else if (event->key() == Qt::Key_Down) m_activeNode->moveBy(0, 20);
            else if (event->key() == Qt::Key_Left) m_activeNode->moveBy(-20, 0);
            else if (event->key() == Qt::Key_Right) m_activeNode->moveBy(20, 0);
        }
        else // move scene
        {
            QGraphicsView::keyPressEvent(event);
        }
        break;

        // zoom in/out
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;

        // Hint mode: select a node vimperator style
    case Qt::Key_F:
        m_showingNodeNumbers = !m_showingNodeNumbers;
        if (!m_showingNodeNumbers)
        {
            showingAllNodeNumbers(false);
            break;
        }

        m_hintNumber.clear();
        showNodeNumbers();
        break;

        // insert new node
    case Qt::Key_Insert:
        insertNode();
        if (m_showingNodeNumbers)
            showNodeNumbers();

        break;

        // used in node selection mode, to select node with numbers/enter
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        if (!m_showingNodeNumbers)
            break;

        m_hintNumber.append(QString::number(event->key()-48));
        showingAllNodeNumbers(false);
        showingNodeNumbersBeginWithNumber(m_hintNumber.toInt(), true);

        break;

        // delete one letter back in node selection
    case Qt::Key_Backspace:
        if (!m_showingNodeNumbers && m_hintNumber.isEmpty())
            break;

        m_hintNumber.remove(m_hintNumber.length()-1,1);
        showNodeNumbers();
        break;

        // in node selection select node if nudenum = enterednum
    case Qt::Key_Return:
    case Qt::Key_Enter:

        if (m_hintNode && m_showingNodeNumbers)
            nodeSelected(m_hintNode);

        break;

        // edit node
    case Qt::Key_F2:
        setActiveNodeEditable();
        break;

        // delete node
    case Qt::Key_Delete:

        if (m_nodeList.size()==1)
        {
            dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                        tr("Last node cannot be deleted."),
                        3000); // millisec
            break;
        }

        if (m_hintNode==m_activeNode)
            m_hintNode=0;

        m_nodeList.removeAll(m_activeNode);
        delete m_activeNode;
        m_activeNode = 0;

        if (m_showingNodeNumbers)
            showNodeNumbers();

        break;

        // add edge to active node
    case Qt::Key_A:

        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("Add edge: select destination node"),
                    4000); // millisec

        m_edgeAdding = true;

        break;

        // add edge to active node
    case Qt::Key_D:

        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("Delete edge: select other end-node"),
                    4000); // millisec

        m_edgeDeleting = true;


        break;

    default:
        QGraphicsView::keyPressEvent(event);
    }
}



void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}


void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).
            mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.2 || factor > 10) return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::setActiveNode(Node *node)
{
    if (m_activeNode!=0)
        m_activeNode->setActive(false);

    m_activeNode = node;
    m_activeNode->setActive();
}

void GraphWidget::insertNode()
{
    double angle(m_activeNode->calculateBiggestAngle());

    qreal length(100);

    QPointF pos(length * cos(angle), length * sin(angle));

    Node *node = new Node(this);
    node->setHtml(QString(""));
    m_scene->addItem(node);
    node->setPos(m_activeNode->sceneBoundingRect().center() +
                 pos -
                 node->boundingRect().center());
    m_nodeList.append(node);

    addEdge(m_activeNode, node);

    setActiveNode(node);
    setActiveNodeEditable();
}

void GraphWidget::showingAllNodeNumbers(const bool &show)
{
    int i =0;
    for (QList<Node *>::const_iterator it = m_nodeList.begin();
         it != m_nodeList.end(); it++, i++)
        dynamic_cast<Node*>(*it)->showNumber(i,show);
}

void GraphWidget::showingNodeNumbersBeginWithNumber(const int &number,
                                                    const bool &show)
{
    int i(0);
    int hit(0);
    for (QList<Node *>::const_iterator it = m_nodeList.begin();
         it != m_nodeList.end(); it++, i++)
    {
        if (i == number)
        {
            hit++;
            dynamic_cast<Node*>(*it)->showNumber(i,show,true);
            m_hintNode = dynamic_cast<Node*>(*it);
            continue;
        }
        if (numberStartsWithNumber(i, number))
        {
            hit++;
            dynamic_cast<Node*>(*it)->showNumber(i,show);
        }
    }
    if (hit==1)
    {
        nodeSelected(m_hintNode);
    }
    else if (hit == 0)
    {
        m_showingNodeNumbers = false;
        showingAllNodeNumbers(false);
    }
}

bool GraphWidget::numberStartsWithNumber(const int &number, const int &prefix)
{
    return (QString::number(number)).startsWith(QString::number(prefix));
}

void GraphWidget::setActiveNodeEditable()
{
    m_editingNode = true;
    m_activeNode->setEditable();
    m_scene->setFocusItem(m_activeNode);
}

void GraphWidget::nodeSelected(Node *node)
{
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

//int GraphWidget::nodeId(Node *node)
//{
//    return m_nodeList.indexOf(node);
//}

void GraphWidget::addEdge(Node *source, Node *destination)
{
    if (source->isConnected(destination))
    {
        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("There is already an edge between these two nodes."),
                    5000); // millisec
    }
    else
    {
        m_scene->addItem(new Edge(source, destination));
    }
}

void GraphWidget::removeEdge(Node *source, Node *destination)
{
    if (!source->isConnected(destination))
    {
        dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                    tr("There no edge between these two nodes."),
                    3000); // millisec
    }
    else
    {
        source->removeEdge(destination);
    }
}

void GraphWidget::showNodeNumbers()
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

void GraphWidget::removeAllNodes()
{
    foreach(Node *node, m_nodeList) delete node;
    m_nodeList.clear();
    m_activeNode = 0;
    m_hintNode = 0;
}

void GraphWidget::addFirstNode()
{
    Node *node1 = new Node(this);
    node1->setHtml(QString("<img src=:/heart.svg width=40 height=40></img>"));
    m_scene->addItem(node1);
    node1->setPos(-10, -10);
    node1->setBorder(false);
    m_nodeList.append(node1);

    m_activeNode = m_nodeList.first();
    m_activeNode->setActive();
}

QList<Edge *> GraphWidget::edges() const
{
    QList<Edge *> list;

    foreach(Node * node, m_nodeList)
        list.append(node->edgesFrom());

    return list;
}
