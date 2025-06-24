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

    qreal targetAngle = std::atan2(vec.y(), vec.x()) * 180 / M_PI;
    qreal currentAngle = rotation();

    qreal angleDiff = targetAngle - currentAngle;
    if (angleDiff > 180) angleDiff -= 360;
    if (angleDiff < -180) angleDiff += 360;

    qreal smoothingFactor = 0.1;
    qreal smoothedAngle = currentAngle + angleDiff * smoothingFactor;

    setRotation(smoothedAngle);
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

bool Car::isStoppedAtTrafficLight() {
    if (paused) return false;
    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        TrafficLight* trafficLight = graph->getTrafficLightAtNode(nextNodeId);
        if (trafficLight) {
            QPointF myPos = pos();
            QPointF lightPos = path[pathIndex + 1];
            qreal distanceToLight = QLineF(myPos, lightPos).length();

            QVector2D movementDir = getCurrentDirection();
            QVector2D pathDir(lightPos - myPos);
            if (pathDir.length() > 0)
                pathDir.normalize();

            qreal alignment = QVector2D::dotProduct(movementDir, pathDir);

            if (distanceToLight < 20.0 && alignment > 0.7 && trafficLight->getState() == TrafficLight::Red) {
                if (!stoppedtrafficlights.contains(nextNodeId)) {
                    trafficLight->incrementCarsStopped();
                    stoppedtrafficlights.insert(nextNodeId);
                }
                return false;
            }
        }
    }
}
bool Car::isStopped() {
    if (paused) return false;
    return !canMove();
}

bool Car::hasCarInFront(double& distToCar) const {
    if (!scene()) return false;
    if (pathIndex >= path.size() - 1) return false;

    QPointF myPos = pos();
    QPointF nextPos = path[pathIndex + 1];
    QVector2D direction(nextPos - myPos);
    if (direction.length() == 0) return false;
    direction.normalize();

    double maxDetectDist = 60.0;
    distToCar = maxDetectDist;

    for (QGraphicsItem* item : scene()->items()) {
        Car* otherCar = dynamic_cast<Car*>(item);
        if (!otherCar || otherCar == this) continue;
        QVector2D toOther(otherCar->pos() - myPos);
        double proj = QVector2D::dotProduct(toOther, direction);
        if (proj > 0 && proj < maxDetectDist) {
            double perp = qAbs(direction.x() * toOther.y() - direction.y() * toOther.x());
            if (perp < 5.0) {
                distToCar = proj;
                return true;
            }
        }
    }
    return false;
}

bool Car::hasPriorityConflict(const QPointF& yieldPosition) const {
    if (!scene()) return false;

    qreal checkRadius = 15.0;
    QRectF zone(yieldPosition - QPointF(checkRadius, checkRadius), QSizeF(2*checkRadius, 2*checkRadius));

    for (QGraphicsItem* item : scene()->items(zone)) {
        Car* other = dynamic_cast<Car*>(item);
        if (other && other != this) {
            QVector2D theirDir = other->getCurrentDirection();
            QVector2D toMe(yieldPosition - other->pos());

            if (QVector2D::dotProduct(theirDir.normalized(), toMe.normalized()) > 0.75) {
                return true;
            }
        }
    }
    return false;
}

bool Car::hasPriorityInRoundabout(const QPointF& yieldPos) const {
    if (!scene()) return false;

    qreal checkRadius = 20.0;
    QRectF detectionZone(yieldPos - QPointF(checkRadius, checkRadius), QSizeF(2*checkRadius, 2*checkRadius));

    for (QGraphicsItem* item : scene()->items(detectionZone)) {
        Car* other = dynamic_cast<Car*>(item);
        if (!other || other == this) continue;

        QVector2D otherDir = other->getCurrentDirection();
        QVector2D toMe(yieldPos - other->pos());

        if (QVector2D::dotProduct(otherDir.normalized(), toMe.normalized()) < -0.5) {
            return true;
        }
    }

    return false;
}



bool Car::canMove() {
    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        Node* nextNode = graph->nodes.value(nextNodeId);

        TrafficLight* trafficLight = graph->getTrafficLightAtNode(nextNodeId);
        if (trafficLight) {
            QPointF nextNodePos = path[pathIndex + 1];
            qreal distanceToLight = QLineF(pos(), nextNodePos).length();
            if (distanceToLight < 15.0 && trafficLight->getState() == TrafficLight::Red) {
                if (!stoppedtrafficlights.contains(nextNodeId)) {
                    trafficLight->incrementCarsStopped();
                    stoppedtrafficlights.insert(nextNodeId);
                }
                return false;
            }
        }

        if (nextNode && nextNode->type == NodeType::Yield) {
            qreal distanceToNext = QLineF(pos(), nextNode->position).length();
            if (distanceToNext < 10.0) {
                if (hasPriorityInRoundabout(nextNode->position)) return false;
            }
        }

    }

    double distToCar = 999;
    if (hasCarInFront(distToCar)) {
        const double safeGap = 25.0;
        if (distToCar < safeGap)
            return false;
    }
    if (hasCarInFront(distToCar) && distToCar < 8.0) return false;

    return true;
}

QVector2D Car::getCurrentDirection() const {
    if (pathIndex < path.size() - 1) {
        return QVector2D(path[pathIndex + 1] - pos()).normalized();
    }
    return QVector2D(0, 0);
}

void Car::move() {
    if (paused) return;
    if (pathIndex >= path.size() - 1) return;
    if (!canMove()) return;

    // Atualiza a estrada e a velocidade a cada passo
    if (pathIndex < pathNodeIds.size()) {
        int nodeId = pathNodeIds[pathIndex];
        if (graph->nodeToRoad.contains(nodeId)) {
            currentRoad = graph->nodeToRoad[nodeId];
            maxSpeed = currentRoad->getMaxSpeed();
        } else {
            currentRoad = nullptr;
            maxSpeed = 1.0;
        }

        qDebug() << "Node:" << nodeId
                 << "Road:" << (currentRoad ? currentRoad->getName() : "None")
                 << "Speed:" << maxSpeed;
    }

    // ---- Movimento real começa aqui ----
    QPointF currentPos = pos();
    QPointF targetPos = path[pathIndex + 1];
    updateRotation(currentPos, targetPos);
    QVector2D direction(targetPos - currentPos);
    qreal segmentLength = direction.length();
    if (segmentLength == 0) {
        pathIndex++;
        return;
    }
    direction.normalize();

    double distToCar = 999.0;
    bool carAhead = hasCarInFront(distToCar);

    const qreal reactionDistance = 50.0;

    qreal targetSpeed = maxSpeed;
    if (carAhead) {
        targetSpeed = maxSpeed * (distToCar / reactionDistance);
        targetSpeed = qBound(minSpeed, targetSpeed, maxSpeed);
    }

    if (currentSpeed < targetSpeed) {
        currentSpeed += accRate;
        currentSpeed = qMin(currentSpeed, targetSpeed);
    } else {
        currentSpeed -= decRate;
        currentSpeed = qMax(currentSpeed, targetSpeed);
    }

    qreal moveDistance = qMin(currentSpeed, segmentLength);
    if (moveDistance < 0.1) return;

    setPos(currentPos + direction.toPointF() * moveDistance);
    totalDistance += moveDistance;

    if (moveDistance >= segmentLength - 0.1)
        pathIndex++;

    if (pathIndex >= path.size() - 1) {
        travelTimeMs = travelTimer.elapsed();
        timer->stop();
        MainWindow::instance()->registerCarFinished(travelTimeMs, totalDistance);
        MainWindow::instance()->removeActiveCar(this);
        if (scene()) scene()->removeItem(this);
        deleteLater();
    }
}

