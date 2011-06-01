#include "graphwidget.h"
#include <QDebug>
#include "node.h"

//#include <QGraphicsTextItem>
#include "edge.h"
#include "math.h"

GraphWidget::GraphWidget(QWidget *parent)
{
    qDebug() << __PRETTY_FUNCTION__;

    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);


//    node1 = new Node(this);
//    scene->addItem(node1);
//    node1->setPos(0, 0);
    Node *node1 = new Node();
    node1->setHtml(QString("salalal"));
    scene->addItem(node1);
    node1->setPos(-100, -100);

    Node *node2 = new Node();
    node2->setHtml(QString("<b>denes</b> is\na really nice person"));
    scene->addItem(node2);
    node2->setPos(100, 100);


//    QGraphicsTextItem *item = new QGraphicsTextItem();
//    item->setPlainText(QString("salalal"));
//    scene->addItem(item);
//    item->setPos(20, 20);
//    item->setFlag(QGraphicsItem::ItemIsMovable);
//    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
//    item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
//    item->setZValue(-1);

//    QGraphicsTextItem *item2 = new QGraphicsTextItem();
//    item2->setHtml(QString("<b>denes</b> is\na really nice person"));
//    scene->addItem(item2);
//    item2->setPos(10, 10);
//    item2->setFlag(QGraphicsItem::ItemIsMovable);
//    item2->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
//    item2->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
//    item2->setZValue(-1);

  scene->addItem(new Edge(node1, node2));
  activeNode = node1;
  activeNode->setFocus();
}


void GraphWidget::keyPressEvent(QKeyEvent *event)
 {
     switch (event->key()) {
//     case Qt::Key_Up:
//         centerNode->moveBy(0, -20);
//         break;
//     case Qt::Key_Down:
//         centerNode->moveBy(0, 20);
//         break;
//     case Qt::Key_Left:
//         centerNode->moveBy(-20, 0);
//         break;
//     case Qt::Key_Right:
//         centerNode->moveBy(20, 0);
//         break;
     case Qt::Key_Plus:
         scaleView(qreal(1.2));
         break;
     case Qt::Key_Minus:
         scaleView(1 / qreal(1.2));
         break;
//     case Qt::Key_Space:
//     case Qt::Key_Enter:
//         foreach (QGraphicsItem *item, scene()->items()) {
//             if (qgraphicsitem_cast<Node *>(item))
//                 item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
//         }
//         break;
     default:
         QGraphicsView::keyPressEvent(event);
     }
 }

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.2 || factor > 10) return;

    scale(scaleFactor, scaleFactor);
}
