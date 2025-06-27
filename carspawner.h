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

    // Controlo de spawning
    void startSpawning(int interval);
    void restart(int interval);
    void stop();
    void pause();
    void resume();

    // Criação de carros
    void spawnCar();
    QVector<Car*>& getCars();

private:
    // Configuração
    int spawnerId;
    Graph* graph;
    QGraphicsScene* scene;
    QTimer* timer;
    QRandomGenerator* rng = QRandomGenerator::global();
    bool paused = false;

    // Carros gerados
    QVector<Car*> cars;

    // Funções auxiliares
    Node* chooseRandomSpawnNode();
    Node* chooseRandomDespawnNode();
};

#endif // CARSPAWNER_H
