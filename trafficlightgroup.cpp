#include "trafficlightgroup.h"

TrafficLightGroup::TrafficLightGroup(QObject* parent)
    : QObject(parent),
    timer(new QTimer(this)),
    opposingGroup(nullptr),
    currentState(TrafficLight::Red),
    greenDuration(6000),
    yellowDuration(3000),
    redDuration(9000),
    isPrimary(false)
{
    connect(timer, &QTimer::timeout, this, &TrafficLightGroup::cycleState);
}

void TrafficLightGroup::addTrafficLight(TrafficLight* light) {
    lights.append(light);
    light->setState(currentState);
}

void TrafficLightGroup::setOpposingGroup(TrafficLightGroup* opposing) {
    opposingGroup = opposing;
    opposing->opposingGroup = this;
}

void TrafficLightGroup::setAsPrimary(bool value) {
    isPrimary = value;
}

void TrafficLightGroup::startCycle() {
    if (!isPrimary || timer->isActive()) return;

    currentState = TrafficLight::Green;
    applyStateToOwnGroup();
    timer->start(greenDuration);
}

void TrafficLightGroup::cycleState() {
    switch (currentState) {
    case TrafficLight::Green:
        currentState = TrafficLight::Yellow;
        applyStateToOwnGroup();
        timer->start(yellowDuration);
        break;

    case TrafficLight::Yellow:
        currentState = TrafficLight::Red;
        applyStateToOwnGroup();
        timer->start(redDuration);

        if (opposingGroup && !opposingGroup->timer->isActive()) {
            opposingGroup->currentState = TrafficLight::Green;
            opposingGroup->applyStateToOwnGroup();
            opposingGroup->timer->start(opposingGroup->greenDuration);
        }
        break;

    case TrafficLight::Red:
        currentState = TrafficLight::Green;
        applyStateToOwnGroup();
        timer->start(greenDuration);
        break;
    }
}

void TrafficLightGroup::applyStateToOwnGroup() {
    for (TrafficLight* light : lights) {
        light->setState(currentState);
    }
}

