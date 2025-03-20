#include "road.h"
#include "intersection.h"

Road::Road(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene)
    : QGraphicsRectItem(x, y, width, height) {
    setBrush(Qt::darkGray);
    scene->addItem(this);
    setFlag(QGraphicsRectItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    }

void Road::checkForIntersections(QGraphicsScene *scene, QList<Intersection *> &intersections) {
    QList<QGraphicsItem *> colliding = collidingItems();  // Verifica itens em colisão

    for (QGraphicsItem *item : colliding) {
        Road *otherRoad = dynamic_cast<Road *>(item);
        if (otherRoad) {
            // Calcula o ponto médio das duas estradas para posicionar a interseção
            qreal centerX = (this->x() + otherRoad->x() + this->rect().width() / 2 + otherRoad->rect().width() / 2) / 2;
            qreal centerY = (this->y() + otherRoad->y() + this->rect().height() / 2 + otherRoad->rect().height() / 2) / 2;

            // Verifica se já existe uma interseção nesse ponto
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
