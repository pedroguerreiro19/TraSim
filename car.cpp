#include "car.h"
#include "trafficlight.h"
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>
#include <qgraphicsscene.h>

Car::Car(int startNode, int endNode, Graph* graph, QGraphicsScene* scene, TrafficLight* trafficLight)
    : graph(graph), pathIndex(0), trafficLight(trafficLight) {

    setRect(0, 0, 10, 10);
    setBrush(Qt::red);

    if (trafficLight == nullptr) {
        qDebug() << "Erro: semáforo não inicializado!";
    }
    QVector<int> nodePath = graph->dijkstra(startNode, endNode);
    qDebug() << "Caminho de nós encontrado:" << nodePath;
    if (nodePath.isEmpty() || nodePath.first() == nodePath.last()) {
        qDebug() << "Caminho inválido!";
        return;
    }
    qDebug() << "Start node:" << startNode << "End node:" << endNode;
    qDebug() << "Caminho encontrado:" << path;

    if (nodePath.isEmpty()) {
        qDebug() << "Erro: Nenhum caminho encontrado de" << startNode << "para" << endNode;
        return;
    }

    for (int nodeId : nodePath) {
        if (graph->nodes.contains(nodeId)) {
            path.append(graph->nodes[nodeId]->position);
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
    if (trafficLight) {
        TrafficLight::State lightState = trafficLight->getState();
        qreal distanceToLight = QLineF(this->pos(), trafficLight->getPosition()).length();
        qDebug() << "Distância até o semáforo: " << distanceToLight;
        qDebug() << "Estado do semáforo: " << (lightState == TrafficLight::Red ? "Vermelho" :
                                                   (lightState == TrafficLight::Yellow ? "Amarelo" : "Verde"));

        if (distanceToLight < 100.0) {
            if (lightState == TrafficLight::Red) {
                qDebug() << "Semáforo vermelho e carro próximo (" << distanceToLight << "px)! Parando.";
                return false;
            }
            if (lightState == TrafficLight::Yellow) {
                qDebug() << "Semáforo amarelo e carro próximo. Pode passar.";
                return true;
            }
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
        qDebug() << "distance:" << distance;
        qDebug() << "movedistance:" << moveDistance;
        qDebug() << "startPos:" << startPos;
        qDebug() << "endPos:" << endPos;



        if (moveDistance < distance) {
            setPos(pos() + direction.toPoint() * moveDistance);
        } else {

            setPos(endPos);
            pathIndex++;

            qDebug() << "Carro passou pelo nó:" << endPos;

            if (pathIndex >= path.size() - 1) {
                qDebug() << "Carro chegou ao destino!";
                timer->stop();
                scene()->removeItem(this);
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
    }
    else {
        isMoving = false;
        qDebug() <<"Carro parou!";
    }
}

QList<QPointF> Car::getPath() const {
    return path;
}

