#include "car.h"
#include "trafficlight.h"
#include "mainwindow.h"
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>

Car::Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene, const QString& imagePath)
    : QObject(), QGraphicsPixmapItem() // usa construtor default!
    , graph(graph), spawnNode(spawnNode), despawnNode(despawnNode)
    , totalDistance(0.0), travelTimeMs(0), paused(false), imagePath(imagePath), pathIndex(0)
{

    QPixmap carPixmap(imagePath);
    if (carPixmap.isNull()) {
        qDebug() << "Erro ao carregar imagem do carro:" << imagePath;
    } else {
        setPixmap(carPixmap.scaled(32, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug() << "Imagem do carro carregada com sucesso:" << imagePath;
    }

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations, false);

    QVector<int> nodePath = graph->dijkstra(spawnNode->id, despawnNode->id);

    if (nodePath.isEmpty() || nodePath.first() == nodePath.last()) {
        return;
    }

    for (int nodeId : nodePath) {
        if (graph->nodes.contains(nodeId)) {
            path.append(graph->nodes[nodeId]->position);
            pathNodeIds.append(nodeId);
        }
    }

    if (!path.isEmpty()) {
        setPos(path.first());
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Car::move);
}

void Car::updateRotation(QPointF from, QPointF to) {
    QVector2D vec(to - from);
    if (vec.length() == 0) return;
    qreal angle = std::atan2(vec.y(), vec.x()) * 180 / M_PI;
    setRotation(angle); // se a imagem estiver “virada para cima”, usa angle - 90
}

void Car::pause() {
    paused = true;
}

void Car::resume() {
    paused = false;
}


void Car::startMoving() {
    if (!path.isEmpty()) {
        travelTimer.start();
        timer->start(16);
        MainWindow::instance()->incrementCarsSpawned();
    }
}

bool Car::isStoppedAtTrafficLight() const {
    if (paused) return false;

    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        TrafficLight* trafficLight = graph->getTrafficLightAtNode(nextNodeId);

        if (trafficLight) {
            TrafficLight::State lightState = trafficLight->getState();
            QPointF nextNodePos = path[pathIndex + 1];
            qreal distanceToLight = QLineF(pos(), nextNodePos).length();

            if (distanceToLight < 15.0 && lightState == TrafficLight::Red) {
                return true;
            }
        }
    }
    return false;
}

bool Car::isStopped() {
    if (paused) return false;

    if (!canMove()) {
        if (pathIndex < pathNodeIds.size() - 1) {
            int nextNodeId = pathNodeIds[pathIndex + 1];
            TrafficLight* trafficLight = graph->getTrafficLightAtNode(nextNodeId);
            QPointF nextNodePos = path[pathIndex + 1];
            qreal distanceToLight = QLineF(pos(), nextNodePos).length();
            if (trafficLight && distanceToLight < 15.0 && trafficLight->getState() == TrafficLight::Red)
                return true;
        }

        if (hasCarInFront())
            return true;
    }
    return false;
}

bool Car::hasCarInFront() const {
    if (!scene()) return false;

    QPointF startPos = pos();
    QPointF endPos;

    if (pathIndex < path.size() - 1) {
        endPos = path[pathIndex + 1];
    } else {
        return false;
    }

    QVector2D direction(endPos - startPos);
    if (direction.length() == 0) return false;
    direction.normalize();

    qreal detectionDistance = 20.0;
    QPointF probePoint = startPos + direction.toPointF() * detectionDistance;

    QList<QGraphicsItem*> itemsInScene = scene()->items(QRectF(probePoint - QPointF(5, 5), QSizeF(10, 10)));

    for (QGraphicsItem* item : itemsInScene) {
        Car* otherCar = dynamic_cast<Car*>(item);
        if (otherCar && otherCar != this) {
            return true;
        }
    }

    return false;
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
                if (!stoppedtrafficlights.contains(nextNodeId)) {
                    trafficLight->incrementCarsStopped();
                    stoppedtrafficlights.insert(nextNodeId);
                }

                return false;
            }
        }
    }
    if (hasCarInFront()) {
        return false;
    }

    return true;
}


void Car::move() {
    if (paused) return;
    if (!canMove()) return;

    if (pathIndex < path.size() - 1) {
        QPointF prevPos = pos();
        QPointF endPos = path[pathIndex + 1];

        QVector2D direction(endPos - prevPos);
        qreal distance = direction.length();
        if (distance > 0) direction.normalize();

        qreal moveDistance = 1.0;
        if (moveDistance < distance) {
            updateRotation(pos(), endPos);
            setPos(pos() + direction.toPointF() * moveDistance);
            totalDistance += moveDistance;
        } else {
            setPos(endPos);
            totalDistance += distance;
            pathIndex++;
        }

        if (pathIndex >= path.size() - 1) {
            travelTimeMs = travelTimer.elapsed();
            timer->stop();

            MainWindow::instance()->registerCarFinished(travelTimeMs, totalDistance);
            MainWindow::instance()->removeActiveCar(this);
            if (scene()) scene()->removeItem(this);
            deleteLater();
        }
    }
}


