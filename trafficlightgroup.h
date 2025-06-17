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
    void setAsPrimary(bool value);
    int greenDuration;
    int yellowDuration;
    int redDuration;
private slots:
    void cycleState();
    void applyStateToOwnGroup();

private:
    bool isPrimary;
    QVector<TrafficLight*> lights;
    QTimer* timer;
    TrafficLightGroup* opposingGroup;
    TrafficLight::State currentState;
};

#endif // TRAFFICLIGHTGROUP_H
