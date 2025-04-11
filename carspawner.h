#ifndef CARSPAWNER_H
#define CARSPAWNER_H

#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QTimer>
#include "graph.h"
#include "car.h"
#include "trafficlight.h"

class CarSpawner : public QObject {
    Q_OBJECT

public:
    CarSpawner(int id, Graph* graph, QGraphicsScene* scene, TrafficLight* trafficLight);
    void setStartAndEnd(int start, int end);
    void startSpawning(int interval);

private slots:
    void spawnCar();

private:
    int spawnerId;
    int startNode;
    int endNode;
    Graph* graph;
    QGraphicsScene* scene;
    QVector<Car*> cars;
    QTimer* timer;
    TrafficLight* trafficLight;
};

#endif // CARSPAWNER_H
