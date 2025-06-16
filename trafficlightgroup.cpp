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

void TrafficLightGroup::setState(TrafficLight::State state) {
    currentState = state;
    for (TrafficLight* light : lights) {
        light->setState(state);
    }
}

void TrafficLightGroup::startCycle() {
    setState(TrafficLight::Green);
    QTimer::singleShot(greenDuration, this, [=]() {
        setState(TrafficLight::Yellow);
        QTimer::singleShot(yellowDuration, this, [=]() {
            setState(TrafficLight::Red);
            if (opposingGroup)
                opposingGroup->startCycle();
        });
    });
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
