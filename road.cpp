#include "Road.h"
#include <QtMath>

Road::Road(int id, RoadType type, const QList<QPointF>& points)
    : id(id), type(type), points(points) {
    calculateBounds();
}

int Road::getId() const {
    return id;
}

RoadType Road::getType() const {
    return type;
}

QList<QPointF> Road::getPoints() const {
    return points;
}

QRectF Road::boundingRect() const {
    return bounds;
}


void Road::calculateBounds() {
    if (points.isEmpty()) {
        bounds = QRectF();
        return;
    }

    qreal minX = points[0].x(), maxX = points[0].x();
    qreal minY = points[0].y(), maxY = points[0].y();

    for (const QPointF& p : points) {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());
        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }

    bounds = QRectF(minX - 10, minY - 10, (maxX - minX) + 20, (maxY - minY) + 20);
}

