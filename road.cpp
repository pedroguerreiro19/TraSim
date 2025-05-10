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

void Road::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    QPen pen(Qt::darkGray, 14, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    if (points.size() < 2) return;

    if (type == RoadType::Curve || type == RoadType::Roundabout) {
        QPainterPath path(points.first());
        for (int i = 1; i < points.size(); ++i) {
            path.lineTo(points[i]);
        }
        if (type == RoadType::Roundabout) {
            path.lineTo(points.first()); // fechar ciclo
        }
        painter->drawPath(path);
    } else { // Straight
        painter->drawLine(points.first(), points.last());
    }
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

