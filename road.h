#ifndef ROAD_H
#define ROAD_H
// Depois organizar código
#include <QGraphicsItem>
#include <QPointF>
#include <QList>
#include <QVector>
#include <QString>

enum class RoadType {
    Highway,
    City,
    Residential
};

class Road : public QGraphicsItem {
public:
    Road(const QVector<int>& nodeList, RoadType type, const QString& name);

    int getId() const;
    RoadType getType() const;
    double getMaxSpeed() const;
    QString getName() const;
    QVector<int> getNodeList() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

private:
    int id;
    RoadType type;
    double maxSpeed;
    QString name;
    QVector<int> nodeList;

    void updateMaxSpeed();
};

#endif // ROAD_H
