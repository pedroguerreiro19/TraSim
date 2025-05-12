#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "carspawner.h"
#include "road.h"
#include "trafficlight.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include <QDir>

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

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupScene() {
    scene->setSceneRect(0, 0, 800, 600);

    QPixmap background("resources/map/map.png");
    QGraphicsPixmapItem* bgItem = scene->addPixmap(background);
    bgItem->setZValue(-1);
    bgItem->setPos(0, 0);

    scene->setSceneRect(background.rect());

    view->centerOn(bgItem);


    Node* node1 = new Node(1, QPointF(225, 81), NodeType::Spawn);
    Node* node2 = new Node(2, QPointF(350, 81), NodeType::Light);
    Node* node3 = new Node(3, QPointF(500, 81), NodeType::Ordinary);
    Node* node4 = new Node(4, QPointF(1018, 81), NodeType::Despawn);



    graph->addNode(1, node1->position, NodeType::Spawn);
    graph->addNode(2, node2->position, NodeType::Light);
    graph->addNode(3, node3->position, NodeType::Ordinary);
    graph->addNode(4, node4->position, NodeType::Despawn);
    graph->addEdge(1,2,1);
    graph->addEdge(2,3,1);
    graph->addEdge(3,4,1);

    TrafficLight *light = new TrafficLight(node2->position.x(), node2->position.y(), graph, node2, scene);
    graph->addTrafficLight(node2->id, light);
    carSpawner = new CarSpawner(1, graph, scene);
    carSpawner->startSpawning(2000);



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








