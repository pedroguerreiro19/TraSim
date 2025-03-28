#include "road.h"
#include "intersection.h"
#include <QPainter>

Road::Road(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene)
    : width(width), height(height), scene(scene) {
    setPos(x, y);
    scene->addItem(this);;
    }
    Road::~Road()
    {
    }

    QRectF Road::boundingRect() const {
        return QRectF(0, 0, width, height);
    }

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->drawRect(0, 0, width, height);
}

QPointF Road::getStart() const {
    return pos();
}

QPointF Road::getEnd() const {
    return pos() + QPointF(width, 0);
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
