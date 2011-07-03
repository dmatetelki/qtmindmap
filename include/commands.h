#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <exception>

#include "graphlogic.h"

class GraphLogic;


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
};

#endif // COMMANDS_H
