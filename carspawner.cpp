#include "carspawner.h"
#include "car.h"
#include "mainwindow.h"
#include <QDebug>
#include <QRandomGenerator>


const QStringList carImages = {
    ":/cars/car1.png",
    ":/cars/car2.png",
    ":/cars/car3.png",
    ":/cars/car4.png",
    ":/cars/car5.png",
    ":/cars/car6.png"
};


CarSpawner::CarSpawner(int id, Graph* graph, QGraphicsScene* scene)
    : spawnerId(id), graph(graph), scene(scene), paused(false) {
    rng = QRandomGenerator::global();
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

QVector<Car*>& CarSpawner::getCars() {
    return cars;
}

void CarSpawner::startSpawning(int interval) {
    if (paused) return;
    timer->start(interval);
}

void CarSpawner::spawnCar() {
    if (graph->spawnNodes.isEmpty() || graph->despawnNodes.isEmpty()) {
        qDebug() << "Não há nodes de spawn ou despawn disponíveis!";
        return;
    }

    Node* spawnNode = chooseRandomSpawnNode();
    Node* despawnNode = chooseRandomDespawnNode();

    int imgIdx = QRandomGenerator::global()->bounded(carImages.size());
    QString imgPath = carImages[imgIdx];

    Car* car = new Car(spawnNode, despawnNode, graph, scene, imgPath);

    cars.append(car);
    scene->addItem(car);
    MainWindow::instance()->addActiveCar(car);
    connect(car, &Car::carSelected, MainWindow::instance(), &MainWindow::displayCarInfo);


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
