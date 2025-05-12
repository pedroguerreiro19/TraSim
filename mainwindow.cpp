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

    QString path = ":/map/map.png";
    QFile file(path);
    qDebug() << "Recurso existe?" << file.exists();

    QPixmap background(":/map/map.png");
    QGraphicsPixmapItem* bgItem = scene->addPixmap(background);
    bgItem->setZValue(-1);
    bgItem->setPos(0, 0);

    scene->setSceneRect(background.rect());

    view->centerOn(bgItem);


    Node* node1 = new Node(1, QPointF(225, 81), NodeType::Spawn);
    Node* node2 = new Node(2, QPointF(350, 81), NodeType::PreLight);
    Node* node3 = new Node(3, QPointF(370, 81), NodeType::Light);
    Node* node4 = new Node(4, QPointF(500, 81), NodeType::Ordinary);
    Node* node5 = new Node(5, QPointF(1018, 81), NodeType::Despawn);



    graph->addNode(1, node1->position, NodeType::Spawn);
    graph->addNode(2, node2->position, NodeType::PreLight);
    graph->addNode(3, node3->position, NodeType::Light);
    graph->addNode(4, node4->position, NodeType::Ordinary);
    graph->addNode(5, node5->position, NodeType::Despawn);
    graph->addEdge(1,2,1);
    graph->addEdge(2,3,1);
    graph->addEdge(3,4,1);
    graph->addEdge(4,5,1);

    TrafficLight *light = new TrafficLight(node2->position.x(), node3->position.y(), graph, node3, scene);
    graph->addTrafficLight(node3->id, light);
    carSpawner = new CarSpawner(1, graph, scene);
    carSpawner->startSpawning(2000);



    scene->update();


    view->setRenderHint(QPainter::Antialiasing);
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = view->mapToScene(event->pos());
    qDebug() << "Clicado em:" << scenePos;
}

void MainWindow::on_actionIniciar_triggered() {
    carSpawner->startSpawning(2000); // Inicia a simulação
}

void MainWindow::on_actionParar_triggered() {
    carSpawner->stop();  // Este método pode precisar ser adicionado em CarSpawner
}

void MainWindow::on_actionReiniciar_triggered() {
    carSpawner->restart(2000); // Reinicia a simulação
}

void MainWindow::on_actionSair_triggered() {
    close(); // Fecha a aplicação
}









