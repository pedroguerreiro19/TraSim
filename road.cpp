#include "road.h"
#include "intersection.h"
#include <QPainter>

Road::Road(int id, qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene, Graph* graph)
    : QGraphicsRectItem(x, y, width, height), id(id), graph(graph) {
    setBrush(Qt::darkGray);
    setPos(x, y);
    scene->addItem(this);
    int startNode = id;
    int endNode = id + 1;

    graph->addNode(startNode, QPointF(x, y));
    graph->addNode(endNode, QPointF(x + width, y + height));
    graph->addEdge(startNode, endNode, width);

    }

Road::~Road() { }

QRectF Road::boundingRect() const {
    return QRectF(0, 0, width, height);  // Ou o cálculo correto para sua estrada
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::gray);
    painter->drawRect(rect());
}

QPointF Road::getStart() const {
    return pos();
}

QPointF Road::getEnd() const {
    return pos() + QPointF(width, 0);
}

void Road::connectTo(Road* other) {
    if (!connectedRoads.contains(other)) {
        connectedRoads.append(other);
        other->connectTo(this); // Conexão bidirecional
    }
}

bool Road::isConnected() {
    return !connectedRoads.isEmpty();
}

QList<Road*> Road::getConnectedRoads() {
    return connectedRoads;
}

void Road::checkForIntersections(QGraphicsScene *scene, QList<Intersection *> &intersections) {
    QList<QGraphicsItem *> colliding = collidingItems();

    for (QGraphicsItem *item : colliding) {
        Road *otherRoad = dynamic_cast<Road *>(item);
        if (otherRoad) {

            qreal centerX = (this->x() + otherRoad->x() + this->rect().width() / 2 + otherRoad->rect().width() / 2) / 2;
            qreal centerY = (this->y() + otherRoad->y() + this->rect().height() / 2 + otherRoad->rect().height() / 2) / 2;


            bool exists = false;
            for (Intersection *inter : intersections) {
                if (qAbs(inter->x() - centerX) < 10 && qAbs(inter->y() - centerY) < 10) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                Intersection *newIntersection = new Intersection(centerX, centerY, 40, scene);
                intersections.append(newIntersection);
                scene->addItem(newIntersection);
            }
        }
    }
}
