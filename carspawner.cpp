#include "carspawner.h"
#include "car.h"
#include "mainwindow.h"
#include <QDebug>

CarSpawner::CarSpawner(int id, Graph* graph, QGraphicsScene* scene)
    : spawnerId(id), graph(graph), scene(scene) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CarSpawner::spawnCar);
}


void CarSpawner::stop() {
    if (timer) timer->stop();
}

void CarSpawner::restart(int interval) {
    stop();
    startSpawning(interval);
}



void CarSpawner::startSpawning(int interval) {
    timer->start(interval);
}

void CarSpawner::spawnCar() {
    if (graph->spawnNodes.isEmpty() || graph->despawnNodes.isEmpty()) {
        qDebug() << "Não há nodes de spawn ou despawn disponíveis!";
        return;
    }

    Node* spawnNode = chooseRandomSpawnNode();
    Node* despawnNode = chooseRandomDespawnNode();

    Car* car = new Car(spawnNode, despawnNode, graph, scene);

    cars.append(car);
    scene->addItem(car);
    MainWindow::instance()->addActiveCar(car);


    car->startMoving();
}

Node* CarSpawner::chooseRandomSpawnNode() {
    int index = rng->bounded(graph->spawnNodes.size());
    return graph->spawnNodes[index];
}

Node* CarSpawner::chooseRandomDespawnNode() {
    int index = rng->bounded(graph->despawnNodes.size());
    return graph->despawnNodes[index];
}
