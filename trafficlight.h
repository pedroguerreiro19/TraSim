#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>

#include "graph.h"


class TrafficLight : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    // Estados possíveis do semáforo
    enum State { Red, Green, Yellow };

    TrafficLight(qreal x, qreal y, Graph* graph, Node* node, QGraphicsScene* scene);

    // Métodos do QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // Gets
    QPointF getPosition() const;
    Node* getNode() const;
    State getState() const;
    int getCarsStopped() const;

    // Controlo de estado
    void setState(State state);
    void pause();
    void resume();

    // Métricas
    void incrementCarsStopped();

private:
    Graph* graph;
    Node* node;
    State currentState;
    int numCarsStopped = 0;
    bool paused = false;
};

#endif // TRAFFICLIGHT_H
