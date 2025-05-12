#include "car.h"
#include "trafficlight.h"
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>

Car::Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene)
    : graph(graph), pathIndex(0), spawnNode(spawnNode), despawnNode(despawnNode) {
    setRect(0, 0, 10, 10);
    setBrush(Qt::red);

    QVector<int> nodePath = graph->dijkstra(spawnNode->id, despawnNode->id);
    qDebug() << "Caminho de nós encontrado:" << nodePath;

    if (nodePath.isEmpty() || nodePath.first() == nodePath.last()) {
        qDebug() << "Caminho inválido!";
        return;
    }

    for (int nodeId : nodePath) {
        if (graph->nodes.contains(nodeId)) {
            path.append(graph->nodes[nodeId]->position);
            pathNodeIds.append(nodeId);
            qDebug() << "Nó" << nodeId << "posição" << graph->nodes[nodeId]->position;
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

bool Car::canMove() {
    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        TrafficLight* trafficLight = graph->getTrafficLightAtNode(nextNodeId);

        if (trafficLight) {
            TrafficLight::State lightState = trafficLight->getState();
            QPointF nextNodePos = path[pathIndex + 1];
            qreal distanceToLight = QLineF(pos(), nextNodePos).length();

            if (distanceToLight < 15.0 && lightState == TrafficLight::Red) {
                qDebug() << "Carro esperando no semáforo em nó:" << nextNodeId;
                return false;
            }
        }
    }
    return true;
}


void Car::move() {
    if (!canMove()) {
        return;
    }

    if (pathIndex < path.size() - 1) {
        QPointF startPos = pos();
        QPointF endPos = path[pathIndex + 1];

        QVector2D direction(endPos - startPos);
        qreal distance = direction.length();
        if (distance > 0) {
            direction.normalize();
        }

        qreal moveDistance = 5.0;

        if (moveDistance < distance) {
            setPos(pos() + direction.toPointF() * moveDistance);
        } else {
            setPos(endPos);
            pathIndex++;
        }

        if (pathIndex >= path.size() - 1) {
            qDebug() << "Carro chegou ao destino!";
            timer->stop();
            if (scene()) {
                scene()->removeItem(this);
            }
            deleteLater();
        }
    }
}

void Car::startMoving() {
    if (!path.isEmpty()) {
        timer->start(100);
        qDebug() << "Carro começando a se mover!";
    }
}
