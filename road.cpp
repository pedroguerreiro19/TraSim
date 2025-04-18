#include "road.h"
#include <QPainter>

Road::Road(int id, qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene)
    : QGraphicsRectItem(x, y, width, height), id(id) {
    setBrush(Qt::darkGray);
    setPos(x, y);
    scene->addItem(this);
    }

Road::~Road() { }

QRectF Road::boundingRect() const {
    return QRectF(0, 0, width, height);
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::gray);
    painter->drawRect(rect());
}

QPointF Road::getStart() const {
    return pos();
}

QPointF Road::getEnd() const {
    return pos() + QPointF(width, 0);
}

void Road::connectTo(Road* other) {
    if (!connectedRoads.contains(other)) {
        connectedRoads.append(other);
        other->connectTo(this);
    }
}

bool Road::isConnected() {
    return !connectedRoads.isEmpty();
}

QList<Road*> Road::getConnectedRoads() {
    return connectedRoads;
}


