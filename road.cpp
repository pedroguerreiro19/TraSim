#include "Road.h"
#include <QtMath>

Road::Road(int id, RoadType type)
    : id(id), type(type) {
}

int Road::getId() const {
    return id;
}

RoadType Road::getType() const {
    return type;
}







