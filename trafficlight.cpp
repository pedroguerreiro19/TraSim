#include "trafficlight.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

TrafficLight::TrafficLight(qreal x, qreal y, Graph* graph, Node* node, QGraphicsScene *scene)
    : graph(graph), node(node), currentState(Red), numCarsStopped(0) {

    setPos(x, y);
    scene->addItem(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrafficLight::updateLight);
    timer->start(5000);
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

void TrafficLight::updateLight() {
    if (paused) return;
    switch (currentState) {
    case Red:
        currentState = Green;
        timer->start(greenDuration);
        break;
    case Green:
        currentState = Yellow;
        timer->start(yellowDuration);
        break;
    case Yellow:
        currentState = Red;
        timer->start(redDuration);
        break;
    }
    update();
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
