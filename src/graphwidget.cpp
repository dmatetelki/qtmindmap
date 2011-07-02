#include "include/graphwidget.h"

#include <QDebug>

#include "include/node.h"
#include "include/edge.h"
#include "include/mainwindow.h"

#include <math.h>

const QColor GraphWidget::m_paperColor(255,255,153);

GraphWidget::GraphWidget(MainWindow *parent)
    : QGraphicsView(parent)
    , m_parent(parent)
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

    m_graphlogic = new GraphLogic(this);
}

void GraphWidget::newScene()
{
    m_graphlogic->removeAllNodes();
    m_graphlogic->addFirstNode();

    this->show();
}

void GraphWidget::closeScene()
{
    m_graphlogic->removeAllNodes();
    this->hide();
}

GraphLogic *GraphWidget::graphLogic() const
{
    return m_graphlogic;
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(qreal(1 / 1.2));
}

// MainWindow::keyPressEvent passes all keyevent to here, except
// Ctrl + m (show/hide mainToolBar) and Ctrl + i (show/hide statusIconsToolbar)
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    // if GraphLogic handles the event then stop.
    if (m_graphlogic->processKeyEvent(event))
        return;

    if (event->key() == Qt::Key_Plus)
    {
        zoomIn();
        return;
    }

    if (event->key() == Qt::Key_Minus)
    {
        zoomOut();
        return;
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
                (event->delta() > 0 ?
                        m_graphlogic->scaleUp() :
                        m_graphlogic->scaleDown()) :
                (event->delta() > 0 ?
                        zoomIn() :
                        zoomOut());
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->fillRect(m_scene->sceneRect(), GraphWidget::m_paperColor);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_scene->sceneRect());
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).
                                        mapRect(QRectF(0, 0, 1, 1)).width();

    // don't allow to scale up/down too much
    if (factor < 0.2 || factor > 10)
        return;

    scale(scaleFactor, scaleFactor);
}
