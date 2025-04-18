#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "carspawner.h"
#include "road.h"
#include "trafficlight.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newRoad(nullptr)
    , graph(new Graph())
{
    ui->setupUi(this);



    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    setupScene();

    TrafficLight *light = new TrafficLight(300, 200, graph, scene);
    carSpawner = new CarSpawner(1, graph, scene, light);
    carSpawner->setStartAndEnd(1, 4);
    carSpawner->startSpawning(2000);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupScene() {
    scene->setSceneRect(0, 0, 800, 600);


    graph->addNode(1, QPointF(618, 270));
    graph->addNode(2, QPointF(350, 270));
    graph->addNode(3, QPointF(351,270));
    graph->addNode(4, QPointF(63, 270));
    graph->addEdge(1,2,1);
    graph->addEdge(2,3,1);
    graph->addEdge(3,4,1);

    Road* r1 = new Road(1, 20, 120, 600, 40, scene);
    Road* r2 = new Road(2, 160, 30, 40, 500, scene);
    roads.append(r1);
    roads.append(r2);
    scene->addItem(r1);
    scene->addItem(r2);
    qDebug() << "Estrada r1 em:" << r1->pos();
    qDebug() << "Estrada r2 em:" << r2->pos();

    scene->update();


    view->setRenderHint(QPainter::Antialiasing);
}




void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = view->mapToScene(event->pos());
    qDebug() << "Clicado em:" << scenePos;
}

bool MainWindow::canPlaceRoad(QPointF position) {
    if (roads.isEmpty()) return false;

    for (Road* road : roads) {
        if (road->contains(road->mapFromScene(position))) {
            return true;
        }
    }
    return false;
}








