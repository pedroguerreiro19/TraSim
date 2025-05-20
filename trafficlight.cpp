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

QRectF TrafficLight::boundingRect() const {
    return QRectF(0, 0, 20, 60);
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
    painter->drawEllipse(2, 2, 16, 16);

    painter->setBrush(yellow);
    painter->drawEllipse(2, 22, 16, 16);

    painter->setBrush(green);
    painter->drawEllipse(2, 42, 16, 16);
}

void TrafficLight::updateLight() {
    if (paused) return;
    switch (currentState) {
    case Red:
        currentState = Green;
        timer->start(6000);
        break;
    case Green:
        currentState = Yellow;
        timer->start(3000);
        break;
    case Yellow:
        currentState = Red;
        qDebug() << "Semáforo ficou vermelho";  // Log para depuração
        timer->start(6000);
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
