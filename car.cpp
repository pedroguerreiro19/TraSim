#include "car.h"
#include "trafficlight.h"
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>

Car::Car(int startNode, int endNode, Graph* graph, QGraphicsScene* scene, TrafficLight* trafficLight)
    : graph(graph), pathIndex(0), trafficLight(trafficLight), hasPassedLight(false),
    startNode(startNode), endNode(endNode) {

    setRect(0, 0, 10, 10);
    setBrush(Qt::red);

    if (!trafficLight) {
        qDebug() << "Aviso: Semáforo não atribuído ao carro!";
    }

    // Calcula o caminho com Dijkstra
    QVector<int> nodePath = graph->dijkstra(startNode, endNode);
    qDebug() << "Caminho de nós encontrado:" << nodePath;

    if (nodePath.isEmpty() || nodePath.first() == nodePath.last()) {
        qDebug() << "Caminho inválido!";
        return;
    }

    // Constrói o caminho de pontos baseado nas posições dos nós
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

bool Car::hasPassedTrafficLight() const {
    return hasPassedLight;
}

bool Car::canMove() {
    if (!trafficLight)
        return true;

    if (hasPassedTrafficLight()) {
        qDebug() << "Carro já passou o semáforo.";
        return true;
    }

    TrafficLight::State lightState = trafficLight->getState();
    qreal distanceToLight = QLineF(this->pos(), trafficLight->getPosition()).length();

    qDebug() << "Distância até o semáforo:" << distanceToLight;
    qDebug() << "Estado do semáforo:"
             << (lightState == TrafficLight::Red ? "Vermelho" :
                     (lightState == TrafficLight::Yellow ? "Amarelo" : "Verde"));

    if (distanceToLight < 100.0) {
        if (lightState == TrafficLight::Red) {
            qDebug() << "Semáforo vermelho. Carro vai parar.";
            return false;
        }
        if (lightState == TrafficLight::Yellow) {
            qDebug() << "Semáforo amarelo. Carro pode passar.";
            return true;
        }
    }

    return true;
}

void Car::move() {
    if (!canMove()) {
        qDebug() << "Carro esperando no semáforo.";
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

        qreal moveDistance = 10.0;

        if (moveDistance < distance) {
            setPos(pos() + direction.toPointF() * moveDistance);
        } else {
            setPos(endPos);
            pathIndex++;

            // Verifica se o carro passou por um nó e atualiza a flag
            if (pathIndex < pathNodeIds.size()) {
                int currentNodeId = pathNodeIds[pathIndex];
                qDebug() << "Carro passou pelo nó:" << currentNodeId;

                if (!hasPassedLight && trafficLight && trafficLight->getNode()) {
                    int lightNodeId = trafficLight->getNode()->id;
                    qDebug() << "Comparando com semáforo no nó ID:" << lightNodeId;

                    if (currentNodeId == lightNodeId) {
                        hasPassedLight = true;
                        qDebug() << "Carro passou do semáforo!";
                    }
                }
            }

            // Chegou ao destino
            if (pathIndex >= path.size() - 1) {
                qDebug() << "Carro chegou ao destino!";
                timer->stop();
                if (scene()) {
                    scene()->removeItem(this);
                }
                deleteLater();
            }
        }

        qDebug() << "Carro movendo para" << pos();
    }
}

void Car::startMoving() {
    if (!path.isEmpty()) {
        timer->start(100);
        qDebug() << "Carro começando a se mover!";
    }

    if (canMove()) {
        isMoving = true;
        qDebug() << "Carro movendo de Node" << startNode << "para Node" << endNode;
    } else {
        isMoving = false;
        qDebug() << "Carro parado no início.";
    }
}

QList<QPointF> Car::getPath() const {
    return path;
}





