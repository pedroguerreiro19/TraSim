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
int Road::getId() const {
    return id;
}

RoadType Road::getType() const {
    return type;
}

QString Road::getName() const {
    return name;
}

QVector<int> Road::getNodeList() const {
    return nodeList;
}

double Road::getMaxSpeed() const {
    return maxSpeed;
}

// Define a velocidade máxima com base no tipo da estrada
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
    }
}




