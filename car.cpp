#include "car.h"
#include <QPainter>


Car::Car(int startNode, int endNode, Graph* graph, QGraphicsScene* scene)
    : graph(graph), pathIndex(0) {
    setRect(0, 0, 10, 10);
    setBrush(Qt::red);


    QVector<int> nodePath = graph->dijkstra(startNode, endNode);

    if (nodePath.isEmpty()) {
        qDebug() << "Erro: Nenhum caminho encontrado de" << startNode << "para" << endNode;
        return;
    }

    for (int nodeId : nodePath) {
        if (graph->nodes.contains(nodeId)) {
            path.append(graph->nodes[nodeId]->position);
        }
    }

    if (!path.isEmpty()) {
        setPos(path.first());
    } else {
        qDebug() << "Erro: Caminho vazio!";
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Car::move);
}

void Car::move() {
    if (pathIndex < path.size() - 1) {
        pathIndex++;
        setPos(path[pathIndex]);
        qDebug() << "Carro movendo para" << path[pathIndex];
    } else {
        timer->stop();
        qDebug() << "Carro chegou ao destino!";
    }
}

void Car::startMoving() {
    if (!path.isEmpty()) {
        timer->start(100);
        qDebug() << "Carro começando a se mover!";
    }
}


