#ifndef CAR_H
#define CAR_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include <QList>
#include "graph.h"
#include "trafficlight.h"


class Car : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Car(int startNode, int endNode, Graph* graph, QGraphicsScene* scene, TrafficLight* trafficLight);
    void startMoving();
    bool canMove(int currentNode);
    QList<QPointF> getPath() const;

private slots:
    void move();

private:
    Graph* graph;
    int startNode;
    int endNode;
    QList<QPointF> path;
    int pathIndex;
    QTimer* timer;
    bool isMoving;
    TrafficLight* trafficLight;
};

#endif // CAR_H
