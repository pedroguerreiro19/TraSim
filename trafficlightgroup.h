#ifndef TRAFFICLIGHTGROUP_H
#define TRAFFICLIGHTGROUP_H

#include <QObject>
#include <QVector>
#include <QTimer>

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

    // Durações dos ciclos
    int greenDuration;
    int yellowDuration;
    int redDuration;

private slots:
    void cycleState();
    void applyStateToOwnGroup();

private:
    QVector<TrafficLight*> lights;         // Semáforos pertencentes ao grupo
    QTimer* timer;                         // Timer para controlar os ciclos
    TrafficLightGroup* opposingGroup;      // Grupo oposto (com semáforos alternados)
    TrafficLight::State currentState;      // Estado atual do grupo
    bool isPrimary;                        // Define se o grupo inicia o ciclo
};

#endif // TRAFFICLIGHTGROUP_H
