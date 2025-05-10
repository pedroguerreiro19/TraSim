#ifndef CAR_H
#define CAR_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include <QList>
#include "graph.h"


class Car : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene);
    void startMoving();
    bool canMove();
    QList<QPointF> getPath() const;

private slots:
    void move();

private:
    Graph* graph;
    Node* spawnNode;
    Node* despawnNode;
    QList<QPointF> path;
    QVector<int> pathNodeIds;
    int pathIndex;
    QTimer* timer;
    bool isMoving;
    bool hasPassedTrafficLight() const;
    bool hasPassedLight;
};

#endif // CAR_H
