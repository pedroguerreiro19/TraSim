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
    m_instance = this;
    ui->setupUi(this);

    this->showMaximized();

    QTimer* tableUpdateTimer = new QTimer(this);
    connect(tableUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCarDataTable);
    tableUpdateTimer->start(100);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    setupScene();

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnCarRandomly);


    connect(ui->spinSpawnInterval, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_spawnIntervalChanged);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupScene() {
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

void MainWindow::on_btnSpawnDespawn_clicked()
{
    if (!spawning) {
        int intervalSeconds = ui->spinSpawnInterval->value();
        int intervalMs = intervalSeconds * 1000;
        spawnTimer->start(intervalMs);
        ui->btnSpawnDespawn->setText("Stop vehicle spawning");
        spawning = true;
    } else {
        spawnTimer->stop();
        ui->btnSpawnDespawn->setText("Start vehicle spawning");
        spawning = false;
    }
}
void MainWindow::on_spawnIntervalChanged(int value) {
    if (spawning) {
        spawnTimer->start(value * 1000); // Atualiza imediatamente
    }
}

void MainWindow::on_btnPauseResumeCars_clicked()
{
    carsPaused = !carsPaused;

    if (carsPaused) {
        ui->btnPauseResumeCars->setText("Continue simulation");
        spawnTimer->stop();
    } else {
        ui->btnPauseResumeCars->setText("Stop simulation");
        if (spawning) {
            int intervalSeconds = ui->spinSpawnInterval->value();
            int intervalMs = intervalSeconds * 1000;
            spawnTimer->start(intervalMs);
        }
    }


    for (Car* car : activeCars) {
        if (carsPaused) {
            car->pause();
        }else{
            car->resume();
        }
    }

    for (auto tl : graph->trafficLights.values()) {
        if (carsPaused){
            tl->pause();
        }else{
            tl->resume();
        }
    }


    for (CarSpawner* spawner : carSpawners) {
        if (carsPaused) {
            spawner->stop();
        }else{
            int intervalSeconds = ui->spinSpawnInterval->value();
            int intervalMs = intervalSeconds * 1000;
            spawnTimer->start(intervalMs);
        }
    }
}

void MainWindow::addActiveCar(Car* car) {
    activeCars.append(car);
    if (activeCars.size() > maxCarsActive)
        maxCarsActive = activeCars.size();
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
    if (!ui) return;
    if (!ui->graphicsView) return;
    if (!scene) return;

    //ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    if (!ui) return;
    if (!ui->graphicsView) return;
    if (!scene) return;

    //ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
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
    int numActive = activeCars.size();
    int numFinished = totalCarsFinished;

    double avgDistance = 0.0;
    if (!allDistances.isEmpty()) {
        double sum = 0.0;
        for (double d : allDistances) sum += d;
        avgDistance = sum / allDistances.size();
    }

    double avgTime = 0.0;
    int validTimes = 0;
    if (!allTravelTimes.isEmpty()) {
        double sum = 0.0;
        for (qint64 t : allTravelTimes) {
            if (t > 0) {
                sum += t;
                validTimes++;
            }
        }
        if (validTimes > 0)
            avgTime = sum / validTimes / 1000.0; // ms -> s
    }

    double avgSpeed = 0.0;
    int validPairs = 0;
    if (!allDistances.isEmpty() && !allTravelTimes.isEmpty()) {
        double sum = 0.0;
        int n = qMin(allDistances.size(), allTravelTimes.size());
        for (int i = 0; i < n; ++i) {
            double t = allTravelTimes[i] / 1000.0;
            if (t > 0) {
                sum += allDistances[i] / t;
                validPairs++;
            }
        }
        if (validPairs > 0)
            avgSpeed = sum / validPairs;
    }

    int carsStoppedNow = 0;
    for (Car* car : activeCars) {
        if (car->isStopped()) {
            carsStoppedNow++;
        }
    }

    double percentStopped = 0.0;
    if (numActive > 0)
        percentStopped = (double)carsStoppedNow / numActive * 100.0;

    percentStoppedHistory.append(percentStopped);

    double meanPercentStopped = 0.0;
    if (!percentStoppedHistory.isEmpty()) {
        double sum = 0.0;
        for (double v : percentStoppedHistory) sum += v;
        meanPercentStopped = sum / percentStoppedHistory.size();
    }
    double trafficFlow = 100.0 - meanPercentStopped;

    ui->carDataTable->clear();
    ui->carDataTable->setColumnCount(2);
    ui->carDataTable->setRowCount(9);
    ui->carDataTable->setHorizontalHeaderLabels({"Metrics", "Value"});

    int row = 0;
    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Active cars:"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(numActive)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Total of cars despawned:"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(numFinished)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Average distance covered per car (m):"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(avgDistance, 'f', 2)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Average travel time per car (s):"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(avgTime, 'f', 2)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Average velocity per car (m/s):"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(avgSpeed, 'f', 2)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Cars stopped at traffic lights or traffic:"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(carsStoppedNow)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Percentage of stopped cars (%):"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(percentStopped, 'f', 1)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Average traffic flow (%):"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(trafficFlow, 'f', 1)));

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Max number of active cars:"));
    ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(maxCarsActive)));

    ui->carDataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


