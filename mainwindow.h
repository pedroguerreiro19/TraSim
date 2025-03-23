#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

class Road;
class Intersection;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleAddRoads();
    void checkForIntersections(Road *road);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QList<Road*> roads;
    QList<Intersection*> intersections;
    Ui::MainWindow *ui;

    bool isAddingRoad;
    Road *newRoad = nullptr;
    QPointF startRoadPos;

    void setupScene();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
};
#endif // MAINWINDOW_H
