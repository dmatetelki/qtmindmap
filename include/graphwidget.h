#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "graphlogic.h"

class MainWindow;
class GraphLogic;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(MainWindow *parent = 0);

    // commands from MainWindow
    void newScene();
    void closeScene();
    bool readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

    static const QColor m_paper;

public slots:

    // commands from MainWindow's MainToolBar's actions
    void insertNode(QKeyEvent *event = 0);
    void removeNode(QKeyEvent *event = 0);
    void editNode(QKeyEvent *event = 0);
    void zoomIn(QKeyEvent *event = 0);
    void zoomOut(QKeyEvent *event = 0);
    void scaleUp(QKeyEvent *event = 0);
    void scaleDown(QKeyEvent *event = 0);
    void nodeColor(QKeyEvent *event = 0);
    void nodeTextColor(QKeyEvent *event = 0);
    void addEdge(QKeyEvent *event = 0);
    void removeEdge(QKeyEvent *event = 0);
    void nodeLoseFocus(QKeyEvent *event = 0);
    void hintMode(QKeyEvent *event = 0);

    // bundled signals from statusIconsToolBar
    void insertPicture(const QString &picture);

    void contentChangedFromLogic();
    void notificationFromLogic(const QString &msg);

signals:

    void contentChanged();
    void notification(const QString &msg);

protected:

    // key dispathcer of the whole program: long and pedant
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    // keymap commands
    void moveUp(QKeyEvent *event);
    void moveDown(QKeyEvent *event);
    void moveLeft(QKeyEvent *event);
    void moveRight(QKeyEvent *event);
    void increment(QKeyEvent *event);
    void decrement(QKeyEvent *event);
    void appendNumber(QKeyEvent *event);
    void delNumber(QKeyEvent *event);
    void applyNumber(QKeyEvent *event);

    void scaleView(qreal scaleFactor);


    MainWindow *m_parent;
    QGraphicsScene *m_scene;
    GraphLogic *m_graphlogic;
    std::map<int, void(GraphWidget::*)(QKeyEvent *)> m_memberMap;


};

#endif // GRAPHWIDGET_H
