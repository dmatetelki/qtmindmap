#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "node.h"


class GraphWidget : public QGraphicsView
{
    Q_OBJECT


public:
    GraphWidget(QWidget *parent = 0);
    QGraphicsScene *getScene();
    void setActiveNode(Node *node);

protected:

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);


private:

    QList<Node *> m_nodeList;
    Node *m_activeNode;
    QGraphicsScene *m_scene;
    bool m_showingNodeNumbers;


};

#endif // GRAPHWIDGET_H
