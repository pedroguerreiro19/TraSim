#ifndef TRAFFICLIGHTGROUP_H
#define TRAFFICLIGHTGROUP_H

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>

#include "trafficlight.h"

class TrafficLightGroup : public QObject {
    Q_OBJECT

public:
    explicit TrafficLightGroup(QObject* parent = nullptr);

    // Gestão do grupo de semáforos
    void addTrafficLight(TrafficLight* light);
    void setOpposingGroup(TrafficLightGroup* opposing);
    void setAsPrimary(bool value);
    void startCycle();
    void pause();
    void resume();

    // Durações dos ciclos
    int greenDuration;
    int yellowDuration;
    int redDuration;

private slots:
    void cycleState();
    void applyStateToOwnGroup();

private:
    QVector<TrafficLight*> lights;
    QTimer* timer;
    TrafficLightGroup* opposingGroup;
    TrafficLight::State currentState;
    bool isPrimary;
    bool paused = false;
    QElapsedTimer stateElapsed;
    int remainingTimeMs = 0;
};

#endif // TRAFFICLIGHTGROUP_H
