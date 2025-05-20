#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "carspawner.h"
#include "trafficlight.h"
#include "car.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include <QDir>


#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>


MainWindow* MainWindow::m_instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , graph(new Graph())
{
    qDebug() << "Antes de setupUi";
    m_instance = this;
    ui->setupUi(this);

    qDebug() << "btnStartSim:" << ui->btnStartSim;
    qDebug() << "btnStopSim:" << ui->btnStopSim;
    qDebug() << "btnDespawnCars:" << ui->btnDespawnCars;
    qDebug() << "graphicsView:" << ui->graphicsView;
    qDebug() << "carDataTable:" << ui->carDataTable;
    qDebug() << "Depois de setupUi";
    this->showMaximized();

    QTimer* tableUpdateTimer = new QTimer(this);
    connect(tableUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCarDataTable);
    tableUpdateTimer->start(100);

    qDebug() << "Antes de criar QGraphicsScene";
    scene = new QGraphicsScene(this);
    qDebug() << "Depois de criar QGraphicsScene";
    ui->graphicsView->setScene(scene);
    qDebug() << "Depois de setScene";

    qDebug() << "Antes do setupScene";
    setupScene();

    qDebug() << "Antes dos connects";
    connect(ui->btnStartSim, &QPushButton::clicked, this, &MainWindow::on_btnStartSim_clicked);
    connect(ui->btnStopSim, &QPushButton::clicked, this, &MainWindow::on_btnStopSim_clicked);
    connect(ui->btnDespawnCars, &QPushButton::clicked, this, &MainWindow::on_btnDespawnCars_clicked);
    qDebug() << "FIM DO CONSTRUTOR";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupScene() {

    QString path = ":/map/map.png";
    QFile file(path);
    qDebug() << "Recurso existe?" << file.exists();

    QPixmap background(":/map/map.png");
    QGraphicsPixmapItem* bgItem = scene->addPixmap(background);
    bgItem->setZValue(-1);
    bgItem->setPos(0, 0);
    scene->setSceneRect(background.rect());

    ui->graphicsView->centerOn(bgItem);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);


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

    TrafficLight *light = new TrafficLight(node3->position.x(), node3->position.y(), graph, node3, scene);
    graph->addTrafficLight(node3->id, light);

    carSpawner = new CarSpawner(1, graph, scene);
    carSpawners.append(carSpawner);
    carSpawner->startSpawning(2000);



    scene->update();


    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    qDebug() << "Clicado em:" << scenePos;
}

void MainWindow::spawnCarRandomly() {
    if (carSpawners.isEmpty()) return;

    int index = QRandomGenerator::global()->bounded(carSpawners.size());
    carSpawners[index]->spawnCar();
}

void MainWindow::on_btnStartSim_clicked()
{
    if (carSpawner)
        carSpawner->startSpawning(2000); // Começa a simulação
}

void MainWindow::on_btnStopSim_clicked()
{
    if (carSpawner)
        carSpawner->stop(); // Para a simulação
}

void MainWindow::addActiveCar(Car* car) {
    activeCars.append(car);
    updateCarDataTable();
}

void MainWindow::removeActiveCar(Car* car) {
    activeCars.removeOne(car);
    updateCarDataTable();
}

void MainWindow::on_btnDespawnCars_clicked()
{

    for (CarSpawner* spawner : carSpawners) {

        QVector<Car*>& cars = spawner->getCars();
        for (Car* car : cars) {
            scene->removeItem(car);
            delete car;
        }
        cars.clear();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (ui->graphicsView && scene)
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if (ui->graphicsView && scene)
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
MainWindow* MainWindow::instance() {
    return m_instance;
}

void MainWindow::incrementCarsSpawned() {
    totalCarsSpawned++;
}
void MainWindow::registerCarFinished(qint64 travelTimeMs, double distance) {
    totalCarsFinished++;
    allTravelTimes.append(travelTimeMs);
    allDistances.append(distance);
}
void MainWindow::saveStatisticsCSV() {
    QFile file("carros_stats.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Carro,Tempo(ms),Distancia\n";
        for (int i = 0; i < allTravelTimes.size(); ++i)
            out << (i+1) << "," << allTravelTimes[i] << "," << allDistances[i] << "\n";
        file.close();
    }
    // Guarda também info dos semáforos:
    QFile f2("semaforos_stats.csv");
    if (f2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out2(&f2);
        out2 << "NodeId,CarrosParados\n";
        for (auto id : graph->trafficLights.keys()) {
            TrafficLight* l = graph->trafficLights[id];
            out2 << id << "," << l->getCarsStopped() << "\n";
        }
        f2.close();
    }
}



void MainWindow::updateCarDataTable()
{
    ui->carDataTable->setColumnCount(3);
    ui->carDataTable->setHorizontalHeaderLabels({"Carro", "Posição X", "Posição Y"});
    ui->carDataTable->setRowCount(activeCars.size());

    for (int i = 0; i < activeCars.size(); ++i) {
        Car* car = activeCars[i];
        ui->carDataTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->carDataTable->setItem(i, 1, new QTableWidgetItem(QString::number(car->pos().x(), 'f', 2)));
        ui->carDataTable->setItem(i, 2, new QTableWidgetItem(QString::number(car->pos().y(), 'f', 2)));
    }
    ui->carDataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


