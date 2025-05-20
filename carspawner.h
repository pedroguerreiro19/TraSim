#ifndef CARSPAWNER_H
#define CARSPAWNER_H

#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QTimer>
#include <QRandomGenerator>
#include <QObject>
#include "graph.h"

class Car;

class CarSpawner : public QObject {
    Q_OBJECT

public:
    CarSpawner(int id, Graph* graph, QGraphicsScene* scene);
    void setStartAndEnd(int start, int end);
    void startSpawning(int interval);
    void stop();
    void restart(int interval);
    void spawnCar();
    void pause();
    void resume();

    QVector<Car*>& getCars() { return cars; }
    QVector<Car*> cars;

private:
    int spawnerId;
    Graph* graph;
    QGraphicsScene* scene;
    QTimer* timer;
    Node* chooseRandomSpawnNode();
    Node* chooseRandomDespawnNode();
    QRandomGenerator* rng = QRandomGenerator::global();
    bool paused = false;

};

#endif // CARSPAWNER_H
