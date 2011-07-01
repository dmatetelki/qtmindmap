#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "node.h"

class MainWindow;

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

public slots:

    // commands from MainWindow's MainToolBar's actions
    void insertNode(QKeyEvent *event = 0);
    void removeNode(QKeyEvent *event = 0);
    void zoomIn(QKeyEvent *event = 0);
    void zoomOut(QKeyEvent *event = 0);
    void scaleUp(QKeyEvent *event = 0);
    void scaleDown(QKeyEvent *event = 0);
    void nodeColor(QKeyEvent *event = 0);
    void nodeTextColor(QKeyEvent *event = 0);
    void addEdge(QKeyEvent *event = 0);
    void removeEdge(QKeyEvent *event = 0);
    void hintMode(QKeyEvent *event = 0);

    // bundled signals from statusIconsToolBar
    void insertPicture(const QString &picture);

    // node reports back it's state change
    void nodeChanged();
    void nodeSelected();
    void nodeEdited(QKeyEvent *event = 0);
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

signals:

    void contentChanged();
    void notification(const QString &msg);

protected:

    // key dispathcer of the whole program: long and pedant
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    Node * nodeFactory();

    void selectNode(Node *node);

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

    void move(const int &x, const int &y, QKeyEvent *event);
    void scaleView(qreal scaleFactor);

    // functions on the edges
    QList<Edge *> allEdges() const;
    void addEdge(Node *source, Node *destination);
    void removeEdge(Node* source, Node *destination);

    // functions on nodes
    void addFirstNode();
    void removeAllNodes();
    void setActiveNode(Node *node);

    // hint mode's nodenumber handling functions
    void showNodeNumbers();
    void showingAllNodeNumbers(const bool &show = true);
    void showingNodeNumbersBeginWithNumber(const int &prefix,
                                           const bool &show = true);


    QList<Node *> m_nodeList;
    MainWindow *m_parent;
    Node *m_activeNode;
    QGraphicsScene *m_scene;
    bool m_showingNodeNumbers;
    QString m_hintNumber;
    Node *m_hintNode;
    bool m_editingNode;
    bool m_edgeAdding;
    bool m_edgeDeleting;

    std::map<int, void(GraphWidget::*)(QKeyEvent *)> m_memberMap;

    static const QColor m_paper;
};

#endif // GRAPHWIDGET_H
