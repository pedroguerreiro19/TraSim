#ifndef ROAD_H
#define ROAD_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>


class Road : public QGraphicsRectItem {
public:
    Road(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene);
};

#endif // ROAD_H
