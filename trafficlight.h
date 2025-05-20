#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QGraphicsItem>
#include <QTimer>
#include "graph.h"



class TrafficLight : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    TrafficLight(qreal x, qreal y, Graph* graph, Node* node, QGraphicsScene *scene);
    QRectF boundingRect() const override;
    enum State { Red, Green, Yellow };
    QPointF getPosition() const;
    Node* getNode() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    TrafficLight::State getState() const;
    void pause();
    void resume();

    void incrementCarsStopped();
    int getCarsStopped() const;

private slots:
    void updateLight();

private:
    Graph* graph;
    State currentState;
    QTimer *timer;
    Node *node;
    int numCarsStopped = 0;
    bool paused = false;
};

#endif // TRAFFICLIGHT_H
