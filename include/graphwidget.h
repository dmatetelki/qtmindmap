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

    // node reports back it's state change
    void nodeSelected(Node *node);
    void nodeMoved(QGraphicsSceneMouseEvent *event);

    // notify MainWindow: a node/edge has changed
    void contentChanged(const bool &changed = true);

    // commands from MainWindow
    void newScene();
    void closeScene();
    bool readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

public slots:

    // commands from MainWindow's MainToolBar's actions
    void insertNode();
    void removeNode();
    void editNode();
    void zoomIn();
    void zoomOut();
    void scaleUp();
    void scaleDown();
    void nodeColor();
    void nodeTextColor();
    void addEdge();
    void removeEdge();
    void nodeLostFocus();
    void hintMode();

    // bundled signals from statusIconsToolBar
    void insertPicture(const QString &picture);

protected:

    // key dispathcer of the whole program: long and pedant
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    // zoom in/out of the view
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
    bool m_contentChanged;
    QString m_fileName;

    static const QColor m_paper;
};

#endif // GRAPHWIDGET_H
