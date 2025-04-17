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
    bool canMove();
    QList<QPointF> getPath() const;

private slots:
    void move();

private:
    Graph* graph;
    int startNode;
    int endNode;
    QList<QPointF> path;
    QVector<int> pathNodeIds;
    int pathIndex;
    QTimer* timer;
    bool isMoving;
    bool hasPassedTrafficLight() const;
    bool hasPassedLight;
    TrafficLight* trafficLight;
};

#endif // CAR_H
