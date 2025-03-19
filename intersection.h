#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>


class Intersection : public QGraphicsEllipseItem{
public:
    Intersection(qreal x, qreal y ,qreal size, QGraphicsScene *scene);
};

#endif // INTERSECTION_H
