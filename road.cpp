#include "road.h"

Road::Road(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene)
    : QGraphicsRectItem(x, y, width, height) {
    setBrush(Qt::darkGray);
    scene->addItem(this);
    }
