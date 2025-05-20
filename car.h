#ifndef CAR_H
#define CAR_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <QList>
#include <QElapsedTimer>
#include "graph.h"


class Car : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene);
    void startMoving();
    bool canMove();
    QList<QPointF> getPath() const;

    qint64 travelTimeMs = 0;
    double totalDistance = 0.0;

private slots:
    void move();

private:
    Graph* graph;
    Node* spawnNode;
    Node* despawnNode;
    QList<QPointF> path;
    QVector<int> pathNodeIds;
    QSet<int> semaforosParados;
    int pathIndex;
    QTimer* timer;
    bool isMoving;
    bool hasCarInFront();
    bool hasPassedTrafficLight() const;
    bool hasPassedLight;
    QElapsedTimer travelTimer;
};

#endif // CAR_H
