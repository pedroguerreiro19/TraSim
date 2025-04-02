#ifndef CAR_H
#define CAR_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QTimer>
#include "graph.h"

class Car : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    Car(int startNode, int endNode, Graph* graph, QGraphicsScene* scene);
    void startMoving();
    QList<QPointF> getPath() const;

private slots:
    void move();

private:
    Graph* graph;
    QList<QPointF> path;
    int pathIndex;
    QTimer* timer;
};

#endif // CAR_H
