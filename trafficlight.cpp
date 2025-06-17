#include "trafficlight.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

TrafficLight::TrafficLight(qreal x, qreal y, Graph* graph, Node* node, QGraphicsScene *scene)
    : graph(graph), node(node), currentState(Red), numCarsStopped(0) {

    setPos(x, y); ;
    setRotation(0);
    scene->addItem(this);
}

void TrafficLight::pause() {
    paused = true;
}

void TrafficLight::resume() {
    paused = false;
}

void TrafficLight::setState(State state) {
    currentState = state;
    update();
}

QRectF TrafficLight::boundingRect() const {
    return QRectF(0, 0, 12, 36);
}

QPointF TrafficLight::getPosition() const {
    return this->pos();
}

Node* TrafficLight::getNode() const {
    return node;
}

void TrafficLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::black);
    painter->drawRect(boundingRect());


    QColor red = (currentState == Red) ? Qt::red : Qt::darkRed;
    QColor yellow = (currentState == Yellow) ? Qt::yellow : Qt::darkYellow;
    QColor green = (currentState == Green) ? Qt::green : Qt::darkGreen;

    painter->setBrush(red);
    painter->drawEllipse(2, 2, 8, 8);

    painter->setBrush(yellow);
    painter->drawEllipse(2, 14, 8, 8);

    painter->setBrush(green);
    painter->drawEllipse(2, 26, 8, 8);
}

TrafficLight::State TrafficLight::getState() const {
    return currentState;
}

void TrafficLight::incrementCarsStopped() {
    numCarsStopped++;
}

int TrafficLight::getCarsStopped() const {
    return numCarsStopped;
}

