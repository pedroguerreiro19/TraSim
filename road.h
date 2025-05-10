#ifndef ROAD_H
#define ROAD_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QList>

enum class RoadType {
    Straight,
    Curve,
    Roundabout
};

class Road : public QGraphicsItem {
public:
    Road(int id, RoadType type, const QList<QPointF>& points);

    int getId() const;
    RoadType getType() const;
    QList<QPointF> getPoints() const;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    int id;
    RoadType type;
    QList<QPointF> points;
    QRectF bounds;
    void calculateBounds();
};

#endif // ROAD_H
