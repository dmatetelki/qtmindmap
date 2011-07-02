#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "graphlogic.h"

class MainWindow;
class GraphLogic;

/** Responsibilities:
  * - Handle scene zoom in/out events
  * - Close scene (clean), new scene (clean & add first node)
  * - Pass key events to GraphLogic
  */
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:

    GraphWidget(MainWindow *parent = 0);

    void newScene();
    void closeScene();

    GraphLogic *graphLogic() const;

    static const QColor m_paperColor;

public slots:

    void zoomIn();
    void zoomOut();

protected:

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    void scaleView(qreal scaleFactor);


    MainWindow *m_parent;
    QGraphicsScene *m_scene;
    GraphLogic *m_graphlogic;
};

#endif // GRAPHWIDGET_H
