#include "intersection.h"

Intersection:: Intersection(qreal x, qreal y ,qreal size, QGraphicsScene *scene)
    : QGraphicsEllipseItem(x,y,size,size) {
    setBrush(Qt::gray);
    scene->addItem(this);
}
