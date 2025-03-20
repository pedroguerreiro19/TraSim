#ifndef ROAD_H
#define ROAD_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>

class Road : public QGraphicsRectItem {
public:
    Road(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
};

#endif // ROAD_H
