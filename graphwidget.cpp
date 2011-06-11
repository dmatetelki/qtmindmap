#include "graphwidget.h"

#include <QDebug>
#include <QStatusBar>


#include "node.h"
#include "edge.h"
#include "math.h"
#include "mainwindow.h"


GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent),
    m_parent(parent),
    m_activeNode(0),
    m_showingNodeNumbers(false),
    m_followNode(0),
    m_editingNode(false)
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

    Node *node1 = new Node(this);
    node1->setHtml(QString("<img src=:/heart.svg width=40 height=40></img>"));
    m_scene->addItem(node1);
    node1->setPos(-10, -10);
    node1->setBorder(false);
    m_nodeList.append(node1);

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


    m_activeNode = m_nodeList.first();
    m_activeNode->setActive();
}

QGraphicsScene *GraphWidget::getScene()
{
    return m_scene;
}


void GraphWidget::keyPressEvent(QKeyEvent *event)
 {
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << event->key();

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


     switch (event->key()) {

     // move sceve, or move node if modkey is ctrl
     case Qt::Key_Up:
     case Qt::Key_Down:
     case Qt::Key_Left:
     case Qt::Key_Right:
         if (event->modifiers() ==  Qt::ControlModifier)
         {
            if (m_activeNode)
            {
                if (event->key() == Qt::Key_Up) m_activeNode->moveBy(0, -20);
                else if (event->key() == Qt::Key_Down) m_activeNode->moveBy(0, 20);
                else if (event->key() == Qt::Key_Left) m_activeNode->moveBy(-20, 0);
                else if (event->key() == Qt::Key_Right) m_activeNode->moveBy(20, 0);
            }
            else
            {
                dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                            tr("No active node."),
                            5000); // millisec
            }
         }
         else
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

     // follow: select a node vimperator style
     case Qt::Key_F:
         m_showingNodeNumbers = !m_showingNodeNumbers;
         if (m_showingNodeNumbers)
             m_followNumber.clear();

         showingAllNodeNumbers(m_showingNodeNumbers);
         if (m_showingNodeNumbers)
         {
             m_nodeList.first()->showNumber(0,true,true);
             m_followNode = m_nodeList.first();
         }
         break;

     // insert new node
     case Qt::Key_Insert:
         if (!m_activeNode)
         {
             dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                         tr("No active node."),
                         5000); // millisec

         }
         else
         {
            insertNode();
         }
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

         m_followNumber.append(QString::number(event->key()-48));
         showingAllNodeNumbers(false);
         showingNodeNumbersBeginWithNumber(m_followNumber.toInt(), true);

         break;

     // delete one letter back in node selection
     case Qt::Key_Backspace:
         if (!m_showingNodeNumbers)
             break;

         if (!m_followNumber.isEmpty())
         {
            m_followNumber.remove(m_followNumber.length()-1,1);
            if (m_followNumber.isEmpty())
            {
                showingAllNodeNumbers(true);
                m_nodeList.first()->showNumber(0,true,true);
                m_followNode = m_nodeList.first();
            }
            else
            {
                showingAllNodeNumbers(false);
                showingNodeNumbersBeginWithNumber(m_followNumber.toInt(), true);
            }
         }
         break;

     // in node selection select node if nudenum = enterednum
     case Qt::Key_Return:
     case Qt::Key_Enter:

         if (m_followNode && m_showingNodeNumbers)
         {
             showingAllNodeNumbers(false);
             if (m_activeNode)
                 m_activeNode->setActive(false);
             m_activeNode = m_followNode;
             m_activeNode->setActive();
             m_showingNodeNumbers = false;
         }

         break;

     // edit node
     case Qt::Key_F2:

         if (m_activeNode)
         {
             m_editingNode = true;
             m_activeNode->setEditable();
             m_scene->setFocusItem(m_activeNode);
         }
         else
         {
           dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                     tr("No active node."),
                     5000); // millisec
         }

         break;

     // delete node
     case Qt::Key_Delete:

         if (m_activeNode)
         {
             if (m_followNode==m_activeNode)
                 m_followNode=0;

             m_nodeList.removeAll(m_activeNode);
             delete m_activeNode;
             m_activeNode = 0;

             if (m_showingNodeNumbers)
             {
                 m_showingNodeNumbers = false;
                 showingAllNodeNumbers(false);
             }
         }
         else
         {
             dynamic_cast<MainWindow *>(m_parent)->getStatusBar()->showMessage(
                         tr("No active node."),
                         5000); // millisec
         }

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
    node->setHtml(QString("new node"));
    m_scene->addItem(node);
    node->setPos(m_activeNode->sceneBoundingRect().center() +
                 pos -
                 node->boundingRect().center()
                 );
    m_nodeList.append(node);

    m_scene->addItem(new Edge(m_activeNode, node));
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
            m_followNode = dynamic_cast<Node*>(*it);
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
        showingAllNodeNumbers(false);
        if (m_activeNode)
            m_activeNode->setActive(false);

        m_activeNode = m_followNode;
        m_activeNode->setActive();
        m_showingNodeNumbers = false;
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
