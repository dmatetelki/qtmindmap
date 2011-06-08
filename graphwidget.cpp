#include "graphwidget.h"
#include <QDebug>
#include "node.h"

//#include <QGraphicsTextItem>
#include "edge.h"
#include "math.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent),
    m_activeNode(0),
    m_showingNodeNumbers(false)
{
    qDebug() << __PRETTY_FUNCTION__;

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
    node1->setHtml(QString("me"));
    m_scene->addItem(node1);
    node1->setPos(-10, -10);
    m_nodeList.append(node1);

    Node *node2 = new Node(this);
    node2->setHtml(QString("work"));
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
    node5->setHtml(QString("joy"));
    m_scene->addItem(node5);
    node5->setPos(-10, 50);
    m_nodeList.append(node5);

    Node *node6 = new Node(this);
    node6->setHtml(QString("rape goats"));
    m_scene->addItem(node6);
    node6->setPos(-10, 100);
    m_nodeList.append(node6);

    m_scene->addItem(new Edge(node1, node2));
    m_scene->addItem(new Edge(node1, node3));
    m_scene->addItem(new Edge(node3, node4));
    m_scene->addItem(new Edge(node1, node5));
    m_scene->addItem(new Edge(node5, node6));

    m_activeNode = m_nodeList.first();
    m_activeNode->setActive(true);
}

QGraphicsScene *GraphWidget::getScene()
{
    return m_scene;
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
 {
    qDebug() << __PRETTY_FUNCTION__;
//    qDebug() << event->key();

     switch (event->key()) {
     case Qt::Key_Up:
         m_activeNode->moveBy(0, -20);
         break;
     case Qt::Key_Down:
         m_activeNode->moveBy(0, 20);
         break;
     case Qt::Key_Left:
         m_activeNode->moveBy(-20, 0);
         break;
     case Qt::Key_Right:
         m_activeNode->moveBy(20, 0);
         break;
     case Qt::Key_Plus:
         scaleView(qreal(1.2));
         break;
     case Qt::Key_Minus:
         scaleView(1 / qreal(1.2));
         break;
     case Qt::Key_F:
        { // need brackets because of local variable
         m_showingNodeNumbers = !m_showingNodeNumbers;
         int i =0;
         for (QList<Node *>::const_iterator it = m_nodeList.begin(); it != m_nodeList.end(); it++, i++)
             dynamic_cast<Node*>(*it)->showNumber(i,m_showingNodeNumbers);
        }
         break;

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

         m_activeNode->setActive(false);
         m_activeNode = m_nodeList[3];
         m_activeNode->setActive(true);

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
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.2 || factor > 10) return;

    scale(scaleFactor, scaleFactor);
}

/// @note this == 0? how is it possible?
void GraphWidget::setActiveNode(Node *node)
{
    qDebug() << __PRETTY_FUNCTION__;

    if (m_activeNode!=0)
        m_activeNode->setActive(false);

    m_activeNode = node;
    m_activeNode->setActive(true);
}
