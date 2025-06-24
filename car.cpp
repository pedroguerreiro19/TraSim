#include "car.h"
#include "trafficlight.h"
#include "mainwindow.h"
#include <QPainter>
#include <QVector2D>
#include <QGraphicsScene>
#include <random>

Car::Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene, const QString& imagePath)
    : QObject(), QGraphicsPixmapItem(), graph(graph), spawnNode(spawnNode), despawnNode(despawnNode), totalDistance(0.0), travelTimeMs(0), paused(false), imagePath(imagePath), pathIndex(0) {

    QPixmap carPixmap(imagePath);
    if (carPixmap.isNull()) {
        qDebug() << "Erro ao carregar imagem do carro:" << imagePath;
    } else {
        setPixmap(carPixmap.scaled(32, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        setOffset(-pixmap().width() / 2.0, -pixmap().height() / 2.0);
        qDebug() << "Imagem do carro carregada com sucesso:" << imagePath;
    }

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations, false);

    QVector<int> nodePath = graph->dijkstra(spawnNode->id, despawnNode->id);
    if (nodePath.isEmpty() || nodePath.first() == nodePath.last()) return;

    for (int nodeId : nodePath) {
        if (graph->nodes.contains(nodeId)) {
            path.append(graph->nodes[nodeId]->position);
            pathNodeIds.append(nodeId);
        }
    }
    if (!path.isEmpty()) setPos(path.first());

    currentSpeed = 0.0;
    maxSpeed = 1.0;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Distribuições
    std::uniform_real_distribution<> minSpeedDist(0.05, 0.15);
    std::uniform_real_distribution<> accDist(0.002, 0.003);
    std::uniform_real_distribution<> decMultDist(1.2, 1.7);

    // Atribuição dos parâmetros com variabilidade
    minSpeed = minSpeedDist(gen);
    accRate = accDist(gen);
    decRate = accRate * decMultDist(gen);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Car::move);
}

void Car::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsPixmapItem::mousePressEvent(event);
    emit carSelected(this);
}

void Car::startMoving() {
    if (!path.isEmpty()) {
        travelTimer.start();
        timer->start(16);
        MainWindow::instance()->incrementCarsSpawned();
    }
}

void Car::pause() { paused = true; }
void Car::resume() { paused = false; }

qint64 Car::getElapsedTravelTimeMs() const { return travelTimer.isValid() ? travelTimer.elapsed() : 0; }

QVector2D Car::getCurrentDirection() const {
    if (pathIndex < path.size() - 1)
        return QVector2D(path[pathIndex + 1] - pos()).normalized();
    return QVector2D(0, 0);
}

double Car::getTotalDistance() const { return totalDistance; }
double Car::getCurrentSpeed() const { return currentSpeed; }
QVector<QPointF> Car::getPath() const { return path; }

void Car::updateRotation(QPointF from, QPointF to) {
    QVector2D vec(to - from);
    if (vec.length() == 0) return;
    qreal targetAngle = std::atan2(vec.y(), vec.x()) * 180 / M_PI;
    qreal currentAngle = rotation();
    qreal angleDiff = targetAngle - currentAngle;
    if (angleDiff > 180) angleDiff -= 360;
    if (angleDiff < -180) angleDiff += 360;
    setRotation(currentAngle + angleDiff * 0.1);
}

bool Car::isStoppedAtTrafficLight() {
    if (paused) return false;
    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        TrafficLight* tl = graph->getTrafficLightAtNode(nextNodeId);
        if (tl) {
            QPointF myPos = pos();
            QPointF lightPos = path[pathIndex + 1];
            qreal dist = QLineF(myPos, lightPos).length();

            QVector2D dir = getCurrentDirection();
            QVector2D toLight(lightPos - myPos);
            if (toLight.length() > 0) toLight.normalize();

            if (dist < 20.0 && QVector2D::dotProduct(dir, toLight) > 0.7 && tl->getState() == TrafficLight::Red) {
                if (!stoppedtrafficlights.contains(nextNodeId)) {
                    tl->incrementCarsStopped();
                    stoppedtrafficlights.insert(nextNodeId);
                }
                return false;
            }
        }
    }
    return true;
}

bool Car::isStopped() {
    if (paused) return false;
    return !canMove();
}

void Car::stop() {
    if (timer && timer->isActive()) timer->stop();
}

bool Car::hasCarInFront(double& distToCar) const {
    if (!scene() || pathIndex >= path.size() - 1) return false;
    QPointF myPos = pos();
    QPointF next = path[pathIndex + 1];
    QVector2D dir(next - myPos);
    if (dir.length() == 0) return false;
    dir.normalize();

    distToCar = 120.0;
    for (QGraphicsItem* item : scene()->items()) {
        Car* other = dynamic_cast<Car*>(item);
        if (!other || other == this) continue;

        QVector2D toOther(other->pos() - myPos);
        double proj = QVector2D::dotProduct(toOther, dir);
        if (proj > 0 && proj < distToCar) {
            double perp = qAbs(dir.x() * toOther.y() - dir.y() * toOther.x());
            if (perp < 5.0) {
                distToCar = proj;
                return true;
            }
        }
    }
    return false;
}

bool Car::hasPriorityConflict(const QPointF& pos) const {
    QRectF zone(pos - QPointF(15, 15), QSizeF(30, 30));
    for (QGraphicsItem* item : scene()->items(zone)) {
        Car* other = dynamic_cast<Car*>(item);
        if (other && other != this) {
            QVector2D theirDir = other->getCurrentDirection();
            QVector2D toMe(pos - other->pos());
            if (QVector2D::dotProduct(theirDir.normalized(), toMe.normalized()) > 0.75) return true;
        }
    }
    return false;
}

bool Car::hasPriorityInRoundabout(const QPointF& yieldPos) const {
    if (!scene()) return false;

    const qreal checkRadius = 40.0;
    QRectF detectionZone(yieldPos - QPointF(checkRadius, checkRadius), QSizeF(2 * checkRadius, 2 * checkRadius));

    for (QGraphicsItem* item : scene()->items(detectionZone)) {
        Car* other = dynamic_cast<Car*>(item);
        if (!other || other == this) continue;

        if (other->pathIndex >= other->path.size() - 1) continue;

        QPointF otherPos = other->pos();
        QVector2D toYield = QVector2D(yieldPos - otherPos);
        QVector2D dir = other->getCurrentDirection();

        if (toYield.length() < checkRadius &&
            QVector2D::dotProduct(dir.normalized(), toYield.normalized()) > 0.5) {

            qreal theirDist = QLineF(otherPos, yieldPos).length();
            qreal myDist = QLineF(pos(), yieldPos).length();

            if (theirDist < myDist - 5.0) {
                return true;
            }
        }
    }

    return false;
}
bool Car::canMove() {
    approachingRedLight = false;

    if (pathIndex < pathNodeIds.size() - 1) {
        int nextNodeId = pathNodeIds[pathIndex + 1];
        Node* nextNode = graph->nodes.value(nextNodeId);

        TrafficLight* tl = graph->getTrafficLightAtNode(nextNodeId);
        if (tl) {
            qreal dist = QLineF(pos(), path[pathIndex + 1]).length();
            if (dist < 100 && tl->getState() == TrafficLight::Red) {
                approachingRedLight = true;
                if (dist < 20) return false;
            }
        }
        if (nextNode && nextNode->type == NodeType::Yield) {
            if (QLineF(pos(), nextNode->position).length() < 30.0 && hasPriorityInRoundabout(nextNode->position))
                return false;
        }
    }

    double distToCar = 999;
    return !(hasCarInFront(distToCar) && distToCar < 25.0);
}

void Car::move() {
    if (paused || pathIndex >= path.size() - 1 || !canMove()) {
        currentSpeed = 0.0;
        return;
    }

    if (pathIndex < pathNodeIds.size()) {
        int nodeId = pathNodeIds[pathIndex];
        if (graph->nodeToRoad.contains(nodeId)) {
            currentRoad = graph->nodeToRoad[nodeId];
            double roadSpeed = currentRoad->getMaxSpeed();

            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<> speedVar(0.8, 1.2);
            maxSpeed = roadSpeed * speedVar(gen);

        } else {
            currentRoad = nullptr;
            maxSpeed = 1.0;
        }
    }

    QPointF currentPos = pos();
    QPointF target = path[pathIndex + 1];
    updateRotation(currentPos, target);
    QVector2D dir(target - currentPos);
    qreal len = dir.length();
    if (len == 0) { pathIndex++; return; }
    dir.normalize();

    double distToCar;
    bool carAhead = hasCarInFront(distToCar);
    qreal targetSpeed = maxSpeed;

    if (approachingRedLight) {
        qreal dist = QLineF(pos(), target).length();
        targetSpeed = maxSpeed * (dist / 90.0);
        targetSpeed = qBound(minSpeed, targetSpeed, maxSpeed);
    } else if (carAhead) {
        targetSpeed = maxSpeed * (distToCar / 90.0);
        targetSpeed = qBound(minSpeed, targetSpeed, maxSpeed);
    }

    if (currentSpeed < targetSpeed) currentSpeed = qMin(currentSpeed + accRate, targetSpeed);
    else currentSpeed = qMax(currentSpeed - decRate, targetSpeed);

    qreal moveDist = qMin(currentSpeed, len);
    if (moveDist < 0.1) return;

    setPos(currentPos + dir.toPointF() * moveDist);
    totalDistance += moveDist;

    if (moveDist >= len - 0.1) pathIndex++;

    if (pathIndex >= path.size() - 1) {
        travelTimeMs = travelTimer.elapsed();
        timer->stop();
        MainWindow::instance()->registerCarFinished(travelTimeMs, totalDistance);
        MainWindow::instance()->removeActiveCar(this);
        if (scene()) scene()->removeItem(this);
        deleteLater();
    }
}

