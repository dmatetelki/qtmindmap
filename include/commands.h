#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <exception>

#include "graphlogic.h"

class GraphLogic;


struct UndoContext
{
    GraphLogic *m_graphLogic;
    Node *m_activeNode;
    Node *m_hintNode;
    QList <Node *> *m_nodeList;
    QPointF m_pos;
    QColor m_color;
    Node *m_source;
    Node *m_destination;
    bool m_secondary;
    qreal m_x;
    qreal m_y;

    UndoContext(GraphLogic *graphLogic = 0,
                Node *activeNode = 0,
                Node *hintNode = 0,
                QList <Node *> *nodeList = 0,
                QPointF pos = QPointF(),
                QColor color = QColor(),
                Node *source = 0,
                Node *destination = 0,
                bool secondary = false,
                qreal x = 0,
                qreal y = 0)
        : m_graphLogic(graphLogic)
        , m_activeNode(activeNode)
        , m_hintNode(hintNode)
        , m_nodeList(nodeList)
        , m_pos(pos)
        , m_color(color)
        , m_source(source)
        , m_destination(destination)
        , m_secondary(secondary)
        , m_x(x)
        , m_y(y)
    {};
};


class InsertNodeCommand : public QUndoCommand
{

public:

    InsertNodeCommand(UndoContext context);
    ~InsertNodeCommand();

    void undo();
    void redo();

private:

    bool m_done;
    UndoContext m_context;

    Node *m_node;
    Node *m_activeNode;
    Edge *m_edge;
};

class RemoveNodeCommand : public QUndoCommand
{

public:

    RemoveNodeCommand(UndoContext context);

    void undo();
    void redo();

private:

    bool m_done;
    UndoContext m_context;

    Node *m_activeNode;
    Node *m_hintNode;

    QList <Node *> m_nodeList;
    QList <Edge *> m_edgeList;
};

class AddEdgeCommand : public QUndoCommand
{

public:

    AddEdgeCommand(UndoContext context);
    ~AddEdgeCommand();

    void undo();
    void redo();

private:

    bool m_done;
    UndoContext m_context;

    Node *m_activeNode;
    Edge *m_edge;
};

class RemoveEdgeCommand : public QUndoCommand
{

public:

    RemoveEdgeCommand(UndoContext context);

    void undo();
    void redo();

private:

    bool m_done;
    UndoContext m_context;

    Node *m_activeNode;
    Edge *m_edge;
};

class MoveCommand : public QUndoCommand
{

public:

    MoveCommand(UndoContext context);

    void undo();
    void redo();

private:

    bool m_done;
    UndoContext m_context;

    QList <Node *> m_nodeList;
};

#endif // COMMANDS_H
