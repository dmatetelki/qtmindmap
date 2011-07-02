#ifndef GRAPHLOGIC_H
#define GRAPHLOGIC_H

#include <QObject>

#include "node.h"
#include "graphwidget.h"


class GraphWidget;

class GraphLogic : public QObject
{
    Q_OBJECT

public:

    explicit GraphLogic(GraphWidget *parent = 0);

    bool processKeyEvent(QKeyEvent *event);
    void addFirstNode();
    void removeAllNodes();

    bool readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

public slots:

    // commands from toolbars:
    void insertNode();  // will be undoable
    void removeNode();  // will be undoable
    void nodeEdited();  // will be undoable
    void scaleUp();     // will be undoable
    void scaleDown();   // will be undoable
    void nodeColor();
    void nodeTextColor();
    void addEdge();     // will be undoable
    void removeEdge();  // will be undoable
    void hintMode();
    void insertPicture(const QString &picture); // will be undoable

    void nodeChanged();
    void nodeSelected();
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

signals:

    void contentChanged();
    void notification(const QString &msg);

private:

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void move(const int &x, const int &y);      // will be undoable
    void setNodeColor(const QColor &color);     // will be undoable
    void setNodeTextColor(const QColor &color); // will be undoable

    // hint mode
    void appendNumber(const int &unm);
    void delNumber();
    void applyNumber();

    Node *nodeFactory();
    void selectNode(Node *node);
    void setActiveNode(Node *node);

    // functions on the edges
    QList<Edge *> allEdges() const;
    void addEdge(Node *source, Node *destination);
    void removeEdge(Node* source, Node *destination);

    // hint mode's nodenumber handling functions
    void showNodeNumbers();
    void showingAllNodeNumbers(const bool &show = true);
    void showingNodeNumbersBeginWithNumber(const int &prefix,
                                           const bool &show = true);

    GraphWidget *m_graphWidget;

    QList<Node *> m_nodeList;
    Node *m_activeNode;
    bool m_showingNodeNumbers;
    QString m_hintNumber;
    Node *m_hintNode;
    bool m_editingNode;
    bool m_edgeAdding;
    bool m_edgeDeleting;

    std::map<int, void(GraphLogic::*)(void)> m_memberMap;

};

#endif // GRAPHLOGIC_H
