#ifndef ROAD_H
#define ROAD_H

#include <QGraphicsItem>
#include <QPointF>
#include <QList>
#include <QVector>
#include <QString>

enum class RoadType {
    Highway,
    City,
    Residential,
    Roundabout
};

// Constantes de velocidade por tipo de estrada
inline constexpr double HIGHWAY_SPEED     = 1.3;
inline constexpr double CITY_SPEED        = 0.80;
inline constexpr double RESIDENTIAL_SPEED = 0.65;

class Road {
public:
    Road(const QVector<int>& nodeList, RoadType type, const QString& name);

    // Gets
    RoadType getType() const;
    double getMaxSpeed() const;
    QString getName() const;
    QVector<int> getNodeList() const;
private:
    // Atributos
    int id;
    RoadType type;
    double maxSpeed;
    QString name;
    QVector<int> nodeList;


    void updateMaxSpeed();
};

#endif // ROAD_H
