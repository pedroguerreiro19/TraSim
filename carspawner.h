#ifndef CARSPAWNER_H
#define CARSPAWNER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsScene>
#include "graph.h"
#include "car.h"

class CarSpawner : public QObject {
    Q_OBJECT
public:
    explicit CarSpawner(int roadId, Graph* graph, QGraphicsScene* scene, QObject* parent = nullptr);
    void startSpawning(int interval = 3000); // Tempo entre spawns

private slots:
    void spawnCar();

private:
    int roadId;
    Graph* graph;
    QGraphicsScene* scene;
    QTimer* spawnTimer;
};

#endif // CARSPAWNER_H

