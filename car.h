#ifndef CAR_H
#define CAR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector>
#include <QSet>
#include "graph.h"

class Car : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Car(Node* spawnNode, Node* despawnNode, Graph* graph, QGraphicsScene* scene, const QString& imagePath);

    // Gets
    QVector<QPointF> getPath() const;
    double getTotalDistance() const;
    double getCurrentSpeed() const;
    qint64 getElapsedTravelTimeMs() const;
    QVector2D getCurrentDirection() const;
    QString getCurrentRoadType() const;

    // Controlo
    bool isStoppedAtTrafficLight();
    bool isStopped();
    bool isTravelTimerRunning = false;
    bool canMove();
    bool isInRoundabout() const { return inRoundabout; }

    void startMoving();
    void pause();
    void resume();
    void stop();

    // Velocidade em km/h
    double getMaxSpeed() const;
    double getCurrentRoadMaxSpeedKmH() const;

    qint64 accumulatedTravelTime = 0;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

signals:
    void carSelected(Car* car);

private slots:
    void move();
    void updateRotation(QPointF from, QPointF to);

private:
    // Dependências
    Graph* graph;
    Node* spawnNode;
    Node* despawnNode;

    // Imagem e caminho
    QString imagePath;
    QPixmap carPixmap;
    QList<QPointF> path;
    QVector<int> pathNodeIds;
    Road* currentRoad = nullptr;

    // Movimento
    int pathIndex = 0;
    qreal currentSpeed = 0.0;
    double totalDistance = 0.0;
    qint64 travelTimeMs = 0;

    // Temporização
    QTimer* timer;
    QElapsedTimer travelTimer;

    // Estado
    bool paused = false;
    bool isMoving = false;
    bool approachingRedLight = false;
    bool hasPassedLight = false;
    bool inRoundabout = false;
    QSet<int> stoppedtrafficlights;

    // Velocidades
    qreal maxSpeed = 1.0;
    qreal minSpeed = 0.2;
    qreal accRate = 0.03;
    qreal decRate = 0.05;


    // Lógica de decisão
    bool hasCarInFront(double& distToCar) const;
    bool hasPriorityInRoundabout(const QPointF& yieldPos) const;
    bool hasPriorityConflict(const QPointF& yieldPosition) const;
    bool hasPassedTrafficLight() const;
};

#endif // CAR_H
