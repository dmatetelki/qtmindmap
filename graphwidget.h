#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMovie>

#include "node.h"

class MainWindow;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(MainWindow *parent = 0);

    void setActiveNode(Node *node);
    void insertNode();
    void setActiveNodeEditable();
    void nodeSelected(Node *node);
    QList<Edge *> edges() const;

    void contentChanged(const bool &changed = true);

    void newScene();
    void closeScene();
    void readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

protected:

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    void scaleView(qreal scaleFactor);
    void showNodeNumbers();
    void showingAllNodeNumbers(const bool &show = true);
    void showingNodeNumbersBeginWithNumber(const int &number,
                                           const bool &show = true);
    bool numberStartsWithNumber(const int &number, const int &prefix);
    qreal calculateBiggestAngle(Node *node);
    void addEdge(Node *source, Node *destination);
    void removeEdge(Node* source, Node *destination);
    void removeAllNodes();
    void addFirstNode();

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
