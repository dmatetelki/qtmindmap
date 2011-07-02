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

    // functions on nodes
    void addFirstNode();
    void removeAllNodes();
    void setActiveNode(Node *node);

    bool readContentFromXmlFile(const QString &fileName);
    void writeContentToXmlFile(const QString &fileName);
    void writeContentToPngFile(const QString &fileName);

    bool editing() const;
    void passKey(QKeyEvent *event = 0);

    // undo-able commands:
    void insertNode();
    void removeNode(const bool &subtree = false);
    void scaleUp(const bool &subtree = false);
    void scaleDown(const bool &subtree = false);
    void setNodeColor(const QColor &color, const bool &subtree = false);
    void setNodeTextColor(const QColor &color, const bool &subtree = false);
    void addEdge();
    void removeEdge();
    void hintMode();
    void insertPicture(const QString &picture);
    void move(const int &x, const int &y, const bool &subtree = false);

    void appendNumber(const int &unm);
    void delNumber();
    void applyNumber();

    void nodeColor(const bool &subtree = false);
    void nodeTextColor(const bool &subtree = false);

public slots:

    void nodeChanged();
    void nodeSelected();
    void nodeEdited(QKeyEvent *event = 0);
    void nodeMoved(QGraphicsSceneMouseEvent *event);
    void nodeLostFocus();

signals:

    void contentChanged();
    void notification(const QString &msg);

private:

    Node *nodeFactory();
    void selectNode(Node *node);


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
};

#endif // GRAPHLOGIC_H
