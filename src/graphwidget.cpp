#include "include/graphwidget.h"

#include <QDebug>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

#include "include/node.h"
#include "include/edge.h"
#include "include/mainwindow.h"

#include <math.h>

const QColor GraphWidget::m_paper(255,255,153);

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


    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Up, &GraphWidget::moveUp));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Down, &GraphWidget::moveDown));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Left, &GraphWidget::moveLeft));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Right, &GraphWidget::moveRight));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Plus, &GraphWidget::increment));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Minus, &GraphWidget::decrement));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_F, &GraphWidget::hintMode));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Insert, &GraphWidget::insertNode));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_0, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_1, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_2, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_3, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_4, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_5, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_6, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_7, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_8, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_9, &GraphWidget::appendNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_0, &GraphWidget::appendNumber));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_F2, &GraphWidget::editNode));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Backspace, &GraphWidget::delNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Return, &GraphWidget::applyNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Enter, &GraphWidget::applyNumber));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_Delete, &GraphWidget::removeNode));

    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_A, &GraphWidget::addEdge));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_D, &GraphWidget::removeEdge));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_C, &GraphWidget::nodeColor));
    m_memberMap.insert(std::pair<int, void(GraphWidget::*)(QKeyEvent *)>
                       (Qt::Key_T, &GraphWidget::nodeTextColor));

    m_graphlogic = new GraphLogic(this);
    connect(m_graphlogic, SIGNAL(contentChanged()),
            this, SLOT(contentChangedFromLogic()));

    connect(m_graphlogic, SIGNAL(notification(QString)),
            this, SLOT(notificationFromLogic(QString)));
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

bool GraphWidget::readContentFromXmlFile(const QString &fileName)
{
    return m_graphlogic->readContentFromXmlFile(fileName);
}

void GraphWidget::writeContentToXmlFile(const QString &fileName)
{
    m_graphlogic->writeContentToXmlFile(fileName);
}

void GraphWidget::writeContentToPngFile(const QString &fileName)
{
    m_graphlogic->writeContentToPngFile(fileName);
}

void GraphWidget::insertNode(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->insertNode();
}

void GraphWidget::removeNode(QKeyEvent *event)
{
    m_graphlogic->removeNode(
                QApplication::keyboardModifiers() & Qt::ControlModifier &&
                QApplication::keyboardModifiers() & Qt::ShiftModifier);
    Q_UNUSED(event)
}

void GraphWidget::editNode(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->nodeEdited();
}

void GraphWidget::zoomIn(QKeyEvent *event)
{
    Q_UNUSED(event)
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut(QKeyEvent *event)
{
    Q_UNUSED(event)
    scaleView(qreal(1 / 1.2));
}

void GraphWidget::scaleUp(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->scaleUp(
            QApplication::keyboardModifiers() & Qt::ControlModifier &&
            QApplication::keyboardModifiers() & Qt::ShiftModifier);
}

void GraphWidget::scaleDown(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->scaleDown(
                QApplication::keyboardModifiers() & Qt::ControlModifier &&
                QApplication::keyboardModifiers() & Qt::ShiftModifier);
}

void GraphWidget::nodeColor(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->nodeColor(
                QApplication::keyboardModifiers() & Qt::ControlModifier &&
                QApplication::keyboardModifiers() & Qt::ShiftModifier);
}

void GraphWidget::nodeTextColor(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->nodeTextColor(
                QApplication::keyboardModifiers() & Qt::ControlModifier &&
                QApplication::keyboardModifiers() & Qt::ShiftModifier);
}

void GraphWidget::addEdge(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->addEdge();
}

void GraphWidget::removeEdge(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->removeEdge();
}

void GraphWidget::nodeLoseFocus(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->nodeLostFocus();
}

void GraphWidget::hintMode(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->hintMode();
}


void GraphWidget::insertPicture(const QString &picture)
{
    Q_UNUSED(picture)
    m_graphlogic->insertPicture(picture);
}

void GraphWidget::contentChangedFromLogic()
{
    emit contentChanged();
}

void GraphWidget::notificationFromLogic(const QString &msg)
{
    emit notification(msg);
}

// All key event arrives here.
// MainWindow::keyPressEvent passes all of them here, except
// Ctrl + m (show/hide mainToolBar) and Ctrl + i (show/hide statusIconsToolbar)
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    // Node lost focus: leaving  edge adding/deleting or Node editing.
    if (event->key() == Qt::Key_Escape)
    {
        nodeLoseFocus();
        return;
    }

    // During Node editing mode, pass every key to the Node's keyPressEvent.
    if (m_graphlogic->editing())
    {
        m_graphlogic->passKey(event);
        return;
    }

    m_memberMap.find(event->key()) != m_memberMap.end() ?
        (this->*m_memberMap[event->key()])(event) :
        QGraphicsView::keyPressEvent(event);
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
                (event->delta() > 0 ?
                        scaleUp() :
                        scaleDown()) :
                (event->delta() > 0 ?
                        zoomIn() :
                        zoomOut());
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    painter->fillRect(m_scene->sceneRect(), GraphWidget::m_paper);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_scene->sceneRect());
}


void GraphWidget::moveUp(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        m_graphlogic->move(0,-20, event->modifiers() & Qt::ShiftModifier) :
        QGraphicsView::keyPressEvent(event);
}

void GraphWidget::moveDown(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        m_graphlogic->move(0,20, event->modifiers() & Qt::ShiftModifier) :
        QGraphicsView::keyPressEvent(event);
}

void GraphWidget::moveLeft(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        m_graphlogic->move(-20,0, event->modifiers() & Qt::ShiftModifier) :
        QGraphicsView::keyPressEvent(event);
}

void GraphWidget::moveRight(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        m_graphlogic->move(20,0, event->modifiers() & Qt::ShiftModifier) :
        QGraphicsView::keyPressEvent(event);
}

void GraphWidget::increment(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        scaleUp(event) :
        zoomIn();
}

void GraphWidget::decrement(QKeyEvent *event)
{
    event->modifiers() & Qt::ControlModifier ?
        scaleDown(event) :
        zoomOut();
}

void GraphWidget::appendNumber(QKeyEvent *event)
{
    m_graphlogic->appendNumber(event->key()-48);
}

void GraphWidget::delNumber(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->delNumber();
}

void GraphWidget::applyNumber(QKeyEvent *event)
{
    Q_UNUSED(event)
    m_graphlogic->applyNumber();
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
