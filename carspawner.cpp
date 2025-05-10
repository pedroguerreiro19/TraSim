#include "carspawner.h"
#include "car.h"
#include <QDebug>

CarSpawner::CarSpawner(int id, Graph* graph, QGraphicsScene* scene)
    : spawnerId(id), graph(graph), scene(scene) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CarSpawner::spawnCar);
}


void CarSpawner::startSpawning(int interval) {
    timer->start(interval);
}

void CarSpawner::spawnCar() {
    // Verificar se existem nodes de spawn e despawn configurados
    if (graph->spawnNodes.isEmpty() || graph->despawnNodes.isEmpty()) {
        qDebug() << "Não há nodes de spawn ou despawn disponíveis!";
        return;
    }

    // Escolher aleatoriamente um node de spawn
    Node* spawnNode = chooseRandomSpawnNode();
    // Escolher aleatoriamente um node de despawn
    Node* despawnNode = chooseRandomDespawnNode();

    // Criar um carro com os nodes de spawn e despawn
    Car* car = new Car(spawnNode, despawnNode, graph, scene);

    // Se o caminho do carro estiver vazio (não encontrou caminho), delete o carro
    if (car->getPath().isEmpty()) {
        delete car;
        return;
    }

    // Adicionar o carro à lista de carros e à cena
    cars.append(car);
    scene->addItem(car);

    // Começar o movimento do carro
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
