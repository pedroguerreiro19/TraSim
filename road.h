#ifndef ROAD_H
#define ROAD_H
#include <QGraphicsItem>
#include <QPointF>
#include <QList>

enum class RoadType {
    Highway,
    City,
    Residential
};

class Road : public QGraphicsItem {
public:
    Road(int id, RoadType type);

    int getId() const;
    RoadType getType() const;


private:
    int id;
    RoadType type;
};

#endif // ROAD_H
