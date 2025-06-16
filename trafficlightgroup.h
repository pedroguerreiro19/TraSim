#ifndef TRAFFICLIGHTGROUP_H
#define TRAFFICLIGHTGROUP_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include "trafficlight.h"

    class TrafficLightGroup : public QObject {
    Q_OBJECT

public:
    TrafficLightGroup(QObject* parent = nullptr);
    void addTrafficLight(TrafficLight* light);
    void setOpposingGroup(TrafficLightGroup* opposing);
    void startCycle();
    void setState(TrafficLight::State state);

    int greenDuration = 6000;
    int yellowDuration = 3000;
    int redDuration = 6000;

private slots:
    void cycleState();

private:
    QVector<TrafficLight*> lights;
    QTimer* timer;
    TrafficLightGroup* opposingGroup;
    TrafficLight::State currentState;
};

#endif // TRAFFICLIGHTGROUP_H
