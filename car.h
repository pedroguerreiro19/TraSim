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
    QList<QPointF> getPath() const;
    bool isStoppedAtTrafficLight() const;
    bool isStopped();
    qint64 travelTimeMs = 0;
    double totalDistance = 0.0;

    void pause();
    void resume();
    void startMoving();
    bool canMove();

private slots:
    void move();

private:
    Graph* graph;
    Node* spawnNode;
    Node* despawnNode;
    QList<QPointF> path;
    QVector<int> pathNodeIds;
    QSet<int> stoppedtrafficlights;
    int pathIndex;
    QTimer* timer;
    bool isMoving;
    bool hasCarInFront() const;
    bool hasPassedTrafficLight() const;
    bool hasPassedLight;
    QElapsedTimer travelTimer;
    bool paused = false;
};

#endif // CAR_H
