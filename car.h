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

    // Controlo
    bool isStoppedAtTrafficLight();
    bool isStopped();
    bool canMove();

    void startMoving();
    void pause();
    void resume();
    void stop();

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
    QSet<int> stoppedtrafficlights;

    // Constantes de movimento
    static constexpr qreal maxSpeed = 1.0;
    static constexpr qreal minSpeed = 0.2;
    static constexpr qreal accRate = 0.03;
    static constexpr qreal decRate = 0.05;

    // Lógica de decisão
    bool hasCarInFront(double& distToCar) const;
    bool hasPriorityInRoundabout(const QPointF& yieldPos) const;
    bool hasPriorityConflict(const QPointF& yieldPosition) const;
    bool hasPassedTrafficLight() const;
};

#endif // CAR_H
