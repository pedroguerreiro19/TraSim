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

void TrafficLightGroup::pause() {
    paused = true;
    if (timer->isActive()) {
        remainingTimeMs = timer->remainingTime();
        if (remainingTimeMs <= 0)
            remainingTimeMs = qMax(1, greenDuration - stateElapsed.elapsed());
        timer->stop();
    }

    for (TrafficLight* light : lights) {
        if (light) light->pause();
    }
}

void TrafficLightGroup::resume() {
    if (!paused) return;

    paused = false;

    for (TrafficLight* light : lights) {
        if (light) light->resume();
    }

    if (!timer->isActive() && remainingTimeMs > 0) {
        timer->start(remainingTimeMs);
        stateElapsed.restart();
        remainingTimeMs = 0;
    }
}


void TrafficLightGroup::setAsPrimary(bool value) {
    isPrimary = value;
}

void TrafficLightGroup::startCycle() {
    if (paused || !isPrimary || timer->isActive()) return;

    currentState = TrafficLight::Green;
    applyStateToOwnGroup();
    timer->start(greenDuration);
}

void TrafficLightGroup::cycleState() {
    if (paused) return;

    switch (currentState) {
    case TrafficLight::Green:
        currentState = TrafficLight::Yellow;
        applyStateToOwnGroup();
        stateElapsed.restart();
        timer->start(yellowDuration);
        break;

    case TrafficLight::Yellow:
        currentState = TrafficLight::Red;
        applyStateToOwnGroup();
        stateElapsed.restart();
        timer->start(redDuration);

        if (opposingGroup && !opposingGroup->paused && !opposingGroup->timer->isActive()) {
            opposingGroup->currentState = TrafficLight::Green;
            opposingGroup->applyStateToOwnGroup();
            opposingGroup->timer->start(opposingGroup->greenDuration);
        }
        break;

    case TrafficLight::Red:
        currentState = TrafficLight::Green;
        applyStateToOwnGroup();
        stateElapsed.restart();
        timer->start(greenDuration);
        break;
    }
}

void TrafficLightGroup::applyStateToOwnGroup() {
    for (TrafficLight* light : lights) {
        light->setState(currentState);
    }
}

