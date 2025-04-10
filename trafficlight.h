#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QGraphicsItem>
#include <QTimer>


class TrafficLight : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    TrafficLight(qreal x, qreal y, QGraphicsScene *scene);
    QRectF boundingRect() const override;
    enum State { Red, Green, Yellow };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    TrafficLight::State getState() const;

private slots:
    void updateLight();

private:
    State currentState;
    QTimer *timer;
};

#endif // TRAFFICLIGHT_H
