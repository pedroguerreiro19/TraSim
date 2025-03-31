#include "carspawner.h"
#include "car.h"

CarSpawner::CarSpawner(int roadId, Graph* graph, QGraphicsScene* scene, QObject* parent)
    : QObject(parent), roadId(roadId), graph(graph), scene(scene) {

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &CarSpawner::spawnCar);
}

void CarSpawner::startSpawning(int interval) {
    spawnTimer->start(interval);
}

void CarSpawner::spawnCar() {
    if (!graph->nodes.contains(roadId) || !graph->nodes.contains(roadId + 1)) {
        qDebug() << "Erro: Nós inválidos para a estrada ID" << roadId;
        return;
    }

    QPointF startPos = graph->nodes[roadId]->position;
    QPointF endPos = graph->nodes[roadId + 1]->position;

    Car* newCar = new Car(roadId, roadId + 1, graph, scene);
    newCar->setPos(startPos);

    scene->addItem(newCar);
    newCar->startMoving();

    qDebug() << "Carro spawnado em" << startPos << " indo para " << endPos;
}
