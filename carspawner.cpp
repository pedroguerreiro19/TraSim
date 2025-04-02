#include "carspawner.h"
#include <QDebug>

CarSpawner::CarSpawner(int id, Graph* graph, QGraphicsScene* scene)
    : spawnerId(id), graph(graph), scene(scene), startNode(-1), endNode(-1) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CarSpawner::spawnCar);
}

void CarSpawner::setStartAndEnd(int start, int end) {
    startNode = start;
    endNode = end;
}

void CarSpawner::startSpawning(int interval) {
    timer->start(interval);
}

void CarSpawner::spawnCar() {
    if (startNode == -1 || endNode == -1) {
        qDebug() << "Start ou End node não configurados!";
        return;
    }

    Car* car = new Car(startNode, endNode, graph, scene);
    if (car->getPath().isEmpty()) {
        delete car;
        return;
    }

    cars.append(car);
    scene->addItem(car);
    car->startMoving();
}
