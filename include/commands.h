#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <exception>

#include "graphlogic.h"

class GraphLogic;


// exceptions:

class NoActiveNodeException : public std::exception
{
public:
    const char* what() const throw();
};

class CannotPlaceNewNodeException : public std::exception
{
public:
    const char* what() const throw();
};

class CannotDeleteBaseNodeException : public std::exception
{
public:
    const char* what() const throw();
};

class BaseNodeCannotBeEdgeTargetException : public std::exception
{
public:
    const char* what() const throw();
};

class EdgeExistsBetweenNodesException : public std::exception
{
public:
    const char* what() const throw();
};


// commands:

class InsertNodeCommand : public QUndoCommand
{

public:

    InsertNodeCommand(GraphLogic *graphLogic);

    void undo();
    void redo();

private:

    GraphLogic *m_graphLogic;

    Node *m_node;
    QPointF m_pos;
    Node *m_activeNode;
    Edge *m_edge;
};

class RemoveNodeCommand : public QUndoCommand
{

public:

    RemoveNodeCommand(GraphLogic *graphLogic);

    void undo();
    void redo();

private:

    GraphLogic *m_graphLogic;

    Node *m_activeNode;
    Node *m_hintNode;

    QList <Node *> m_nodeList;
    QList <Edge *> m_edgeList;
};

class AddEdgeCommand : public QUndoCommand
{

public:

    AddEdgeCommand(GraphLogic *graphLogic, Node *source, Node *destinaion);

    void undo();
    void redo();

private:

    GraphLogic *m_graphLogic;

    Node *m_activeNode;
    Node *m_source;
    Node *m_destination;
    Edge *m_edge;
};

class RemoveEdgeCommand : public QUndoCommand
{

public:

    RemoveEdgeCommand(GraphLogic *graphLogic, Node *source, Node *destinaion);

    void undo();
    void redo();

private:

    GraphLogic *m_graphLogic;

    Node *m_activeNode;
    Node *m_source;
    Node *m_destination;
    Edge *m_edge;
};

#endif // COMMANDS_H
