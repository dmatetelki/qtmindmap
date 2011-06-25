#define private public

#include "algorithmtests.h"

#include <QDebug>

#include "include/mainwindow.h"
#include "include/graphwidget.h"
#include "include/node.h"
#include "include/edge.h"

static const double Pi = 3.14159265358979323846264338327950288419717;

AlgorithmTests::AlgorithmTests(QObject *parent) :
    QObject(parent)
{
}

/** edge->paint() calculates the m_angle of th edge,
  * but it is skipped so it must be done manually
  */
double angleOfPoints(const QPointF &a, const QPointF &b)
{
    QLineF line(a, b);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2 * Pi - angle;

    return angle;
}

void AlgorithmTests::calculateBiggestAngle()
{
    MainWindow *mainWindow = new MainWindow;
    GraphWidget *graphWidget = new GraphWidget(mainWindow);

    // no edges
    Node *node1 = new Node(graphWidget);
    node1->setPos(0,0);
    QCOMPARE(node1->calculateBiggestAngle(), Pi * 1.5);


    // one egde
    // 1
    Node *node2 = new Node(graphWidget);
    node2->setPos(30,0);

    Edge *edge1 = new Edge(node1, node2);
    edge1->m_angle = angleOfPoints(node1->pos(), node2->pos());

    QCOMPARE(edge1->getAngle(), 2 * Pi);
    QCOMPARE(node1->calculateBiggestAngle(), - Pi);
    QCOMPARE(node2->calculateBiggestAngle(), double(0));

    // 2
    node2->setPos(30,30);
    edge1->m_angle = angleOfPoints(node1->pos(), node2->pos()); // 45

    QCOMPARE(edge1->getAngle(), 1.75 * Pi);
    QCOMPARE(node1->calculateBiggestAngle(), - 0.75 * Pi);
    QCOMPARE(node2->calculateBiggestAngle(), 0.25 * Pi);


    // more edges
    node2->setPos(30,0);
    edge1->m_angle = angleOfPoints(node1->pos(), node2->pos());

    Node *node3 = new Node(graphWidget);
    node3->setPos(-30,0);
    Edge *edge2 = new Edge(node1, node3);
    edge2->m_angle = angleOfPoints(node1->pos(), node3->pos());

    Node *node4 = new Node(graphWidget);
    node4->setPos(0, -30);
    Edge *edge3 = new Edge(node1, node4);
    edge3->m_angle = angleOfPoints(node1->pos(), node4->pos());

    QCOMPARE(edge1->getAngle(), 2 * Pi);
    QCOMPARE(edge2->getAngle(), Pi);
    QCOMPARE(edge3->getAngle(), 0.5 * Pi);
    QCOMPARE(node1->calculateBiggestAngle(), 0.5 * Pi);

    delete node1;
    delete node2;
    delete node3;
    delete node4;

    delete graphWidget;
    delete mainWindow;
}


QTEST_MAIN(AlgorithmTests)
