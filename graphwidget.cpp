#include "graphwidget.h"
#include <QDebug>
#include "node.h"

//#include <QGraphicsTextItem>
#include "edge.h"
#include "math.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QGraphicsView(parent)
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

    Node *node1 = new Node();
    node1->setHtml(QString("me"));
    m_scene->addItem(node1);
    node1->setPos(-10, -10);

    Node *node2 = new Node();
    node2->setHtml(QString("work"));
    m_scene->addItem(node2);
    node2->setPos(60, -10);

    Node *node3 = new Node();
    node3->setHtml(QString("read"));
    m_scene->addItem(node3);
    node3->setPos(-70, -10);

    Node *node4 = new Node();
    node4->setHtml(QString("pragmatic programmer"));
    m_scene->addItem(node4);
    node4->setPos(-120, -80);

    Node *node5 = new Node();
    node5->setHtml(QString("joy"));
    m_scene->addItem(node5);
    node5->setPos(-10, 50);

    Node *node6 = new Node();
    node6->setHtml(QString("rape goats"));
    m_scene->addItem(node6);
    node6->setPos(-10, 100);

    m_scene->addItem(new Edge(node1, node2));
    m_scene->addItem(new Edge(node1, node3));
    m_scene->addItem(new Edge(node3, node4));
    m_scene->addItem(new Edge(node1, node5));
    m_scene->addItem(new Edge(node5, node6));

    m_activeNode = node1;
    m_activeNode->setFocus();

}

QGraphicsScene *GraphWidget::getScene()
{
    return m_scene;
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

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
 {
     Q_UNUSED(rect);

     // Shadow
     QRectF sceneRect = this->sceneRect();
//     QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
//     QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
//     if (rightShadow.intersects(rect) || rightShadow.contains(rect))
//         painter->fillRect(rightShadow, Qt::darkGray);
//     if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
//         painter->fillRect(bottomShadow, Qt::darkGray);

     // Fill
     QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
     gradient.setColorAt(0, Qt::white);
     gradient.setColorAt(1, Qt::lightGray);
     painter->fillRect(rect.intersect(sceneRect), gradient);
     painter->setBrush(Qt::NoBrush);
     painter->drawRect(sceneRect);

//     // Text
//     QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
//                     sceneRect.width() - 4, sceneRect.height() - 4);
//     QString message(tr("Click and drag the nodes around, and zoom with the mouse "
//                        "wheel or the '+' and '-' keys"));

//     QFont font = painter->font();
//     font.setBold(true);
//     font.setPointSize(14);
//     painter->setFont(font);
//     painter->setPen(Qt::lightGray);
//     painter->drawText(textRect.translated(2, 2), message);
//     painter->setPen(Qt::black);
//     painter->drawText(textRect, message);
 }


void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.2 || factor > 10) return;

    scale(scaleFactor, scaleFactor);
}
