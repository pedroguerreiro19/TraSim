#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"
#include "carspawner.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QHash>
#include <QPointF>

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


private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QList<Road*> roads;
    QList<Intersection*> intersections;
    Ui::MainWindow *ui;

    bool canPlaceRoad(QPointF position);
    Road *newRoad = nullptr;
    CarSpawner* carSpawner;
    Graph *graph;


    void setupScene();

    void mousePressEvent(QMouseEvent *event) override;

};
#endif // MAINWINDOW_H
