#ifndef ROAD_H
#define ROAD_H
#include "intersection.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>


class Road : public QGraphicsRectItem {
public:
    Road(int id, qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene);
    ~Road() override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void checkForIntersections(QGraphicsScene *scene, QList<Intersection *> &intersections);

    void connectTo(Road* other);
    bool isConnected();
    QList<Road*> getConnectedRoads();

    QPointF getStart() const;
    QPointF getEnd() const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        QGraphicsRectItem::mouseMoveEvent(event);
    }

private:
    qreal width;
    qreal height;
    int id;
    QList<Road*> connectedRoads;
    QGraphicsScene* scene;
};

#endif // ROAD_H
