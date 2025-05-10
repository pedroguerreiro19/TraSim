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

    QPixmap background("map.png");
    QGraphicsPixmapItem* bgItem = scene->addPixmap(background);
    bgItem->setZValue(-1);
    bgItem->setPos(0, 0);

    scene->setSceneRect(background.rect());

    view->centerOn(bgItem);

    graph->addNode(1, QPointF(225, 81));
    graph->addNode(2, QPointF(441, 81));
    graph->addNode(3, QPointF(1018,81));
    graph->addNode(4, QPointF(63, 270));
    graph->addEdge(1,2,1);
    graph->addEdge(2,3,1);
    graph->addEdge(3,4,1);



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








