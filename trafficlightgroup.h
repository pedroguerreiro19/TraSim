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

private slots:
    void cycleState();

private:
    QVector<TrafficLight*> lights;
    QTimer* timer;
    TrafficLightGroup* opposingGroup;
    TrafficLight::State currentState;
};

#endif // TRAFFICLIGHTGROUP_H
