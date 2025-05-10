#ifndef CARSPAWNER_H
#define CARSPAWNER_H

#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QTimer>
#include <QRandomGenerator>
#include "graph.h"

class Car;

class CarSpawner : public QObject {
    Q_OBJECT

public:
    CarSpawner(int id, Graph* graph, QGraphicsScene* scene);
    void setStartAndEnd(int start, int end);
    void startSpawning(int interval);

private:
    int spawnerId;
    Graph* graph;
    QGraphicsScene* scene;
    QTimer* timer;
    QVector<Car*> cars;
    void spawnCar();
    Node* chooseRandomSpawnNode();
    Node* chooseRandomDespawnNode();
    QRandomGenerator* rng = QRandomGenerator::global();

};

#endif // CARSPAWNER_H
