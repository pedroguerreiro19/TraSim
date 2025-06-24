#include "road.h"
#include <QPainter>


constexpr double highwayspeed = 1.3;
constexpr double cityspeed = 0.90;
constexpr double residentialspeed = 0.65;


Road::Road(const QVector<int>& nodeList, RoadType type, const QString& name)
    : nodeList(nodeList), type(type), name(name) {
    static int nextId = 1;
    id = nextId++;
    updateMaxSpeed();
}

int Road::getId() const { return id; }
RoadType Road::getType() const { return type; }
QString Road::getName() const { return name; }
QVector<int> Road::getNodeList() const { return nodeList; }

double Road::getMaxSpeed() const { return maxSpeed; }

void Road::updateMaxSpeed() {
    switch (type) {
    case RoadType::Highway:
        maxSpeed = highwayspeed;
        break;
    case RoadType::City:
        maxSpeed = cityspeed;
        break;
    case RoadType::Residential:
        maxSpeed = residentialspeed;
        break;
    }
}

void Road::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (nodeList.isEmpty()) return;

}

QRectF Road::boundingRect() const {
    return QRectF();
}




