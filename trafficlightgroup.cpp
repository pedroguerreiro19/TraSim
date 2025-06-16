#include "trafficlightgroup.h"

TrafficLightGroup::TrafficLightGroup(QObject* parent)
    : QObject(parent), timer(new QTimer(this)), opposingGroup(nullptr), currentState(TrafficLight::Red) {
    connect(timer, &QTimer::timeout, this, &TrafficLightGroup::cycleState);
}

void TrafficLightGroup::addTrafficLight(TrafficLight* light) {
    lights.append(light);
    light->setState(currentState);
}

void TrafficLightGroup::setOpposingGroup(TrafficLightGroup* opposing) {
    opposingGroup = opposing;
}

void TrafficLightGroup::startCycle() {
    cycleState();
}

void TrafficLightGroup::cycleState() {
    if (currentState == TrafficLight::Red) {
        currentState = TrafficLight::Green;
        timer->start(6000);
    } else if (currentState == TrafficLight::Green) {
        currentState = TrafficLight::Yellow;
        timer->start(3000);
    } else if (currentState == TrafficLight::Yellow) {
        currentState = TrafficLight::Red;
        timer->start(6000);
    }

    for (TrafficLight* light : lights)
        light->setState(currentState);

    if (opposingGroup) {
        TrafficLight::State opposite;
        if (currentState == TrafficLight::Red)       opposite = TrafficLight::Green;
        else if (currentState == TrafficLight::Green) opposite = TrafficLight::Red;
        else                                          opposite = TrafficLight::Yellow;

        for (TrafficLight* light : opposingGroup->lights)
            light->setState(opposite);
    }
}
