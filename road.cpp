#include "road.h"
#include <QPainter>

Road::Road(const QVector<int>& nodeList, RoadType type, const QString& name)
    : nodeList(nodeList), type(type), name(name)
{
    static int nextId = 1;
    id = nextId++;
    updateMaxSpeed();
}

// Gets
RoadType Road::getType() const {
    return type;
}

double Road::getMaxSpeed() const {
    return maxSpeed;
}

void Road::updateMaxSpeed() {
    switch (type) {
    case RoadType::Highway:
        maxSpeed = HIGHWAY_SPEED;
        break;
    case RoadType::City:
        maxSpeed = CITY_SPEED;
        break;
    case RoadType::Residential:
        maxSpeed = RESIDENTIAL_SPEED;
        break;
    case RoadType::Roundabout:
        maxSpeed = CITY_SPEED;
        break;
    }
}




