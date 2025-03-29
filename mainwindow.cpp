#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "road.h"
#include "intersection.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isAddingRoad(false)
    , isAddingCurve(false)
    , newRoad(nullptr)
    , graph(new Graph())
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    QPushButton *btnAddRoad = new QPushButton("Adicionar Estrada Reta", this);
    connect(btnAddRoad, &QPushButton::clicked, this, &MainWindow::toggleAddRoads);
    mainLayout->addWidget(view);
    mainLayout->addWidget(btnAddRoad);

    QPushButton *btnAddCurve = new QPushButton("Adicionar Curva", this);
    connect(btnAddCurve, &QPushButton::clicked, this, &MainWindow::toggleAddCurves);
    mainLayout->addWidget(btnAddCurve);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setupScene();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupScene() {
    scene->setSceneRect(0, 0, 800, 600);


    Road* r1 = new Road(1, 100, 250, 600, 40, scene, graph);
    Road* r2 = new Road(2, 350, 50, 40, 500, scene, graph);
    roads.append(r1);
    roads.append(r2);
    scene->addItem(r1);
    scene->addItem(r2);
    qDebug() << "Estrada r1 em:" << r1->pos();
    qDebug() << "Estrada r2 em:" << r2->pos();

    scene->update();


    intersections.append(new Intersection(350, 250, 10, scene));




    view->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::toggleAddRoads() {
    isAddingRoad = !isAddingRoad;
    isAddingCurve = false;
    setCursor(isAddingRoad ? Qt::CrossCursor : Qt::ArrowCursor);
    qDebug() << (isAddingRoad ? "Modo Adicionar Estrada Ativado" : "Modo Adicionar Estrada Desativado");
}

void MainWindow::toggleAddCurves() {
    isAddingCurve = !isAddingCurve;
    isAddingRoad = false;
    setCursor(isAddingCurve ? Qt::CrossCursor : Qt::ArrowCursor);
    qDebug() << (isAddingCurve ? "Modo Adicionar Curva Ativado" : "Modo Adicionar Curva Desativado");
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (isAddingRoad) {
        QPointF scenePos = view->mapToScene(event->pos());


        bool roadPlaced = false;
        for (Road* road : roads) {
            if (road->contains(road->mapFromScene(scenePos))) {
                if (!newRoad) {
                    newRoad = new Road(roads.size() + 1, scenePos.x(), scenePos.y(), 100, 10, scene, graph);
                    roads.append(newRoad);
                    qDebug() << "Estrada criada!";
                    roadPlaced = true;
                } else {
                    newRoad->setFlag(QGraphicsItem::ItemIsMovable, true);

                    newRoad->connectTo(road);
                    qDebug() << "Estrada conectada!";
                    break;
                }
            }
        }

        if (!roadPlaced) {
            qDebug() << "Estrada não pode ser colocada aqui!";
            return;
        }

        newRoad = nullptr;
        isAddingRoad = false;
        setCursor(Qt::ArrowCursor);
    }
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
void MainWindow::checkForIntersections(Road *road) {
    for (Road *existingRoad : roads) {
        if (existingRoad != road && existingRoad->collidesWithItem(road)) {
            QPointF intersectionPoint = road->boundingRect().center();


            bool exists = false;
            for (Intersection* inter : intersections) {
                if (inter->pos() == intersectionPoint) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                intersections.append(new Intersection(intersectionPoint.x(), intersectionPoint.y(), 10, scene));
                qDebug() << "Interseção adicionada em:" << intersectionPoint;


                // graph[intersectionPoint] = {};
            }
        }
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!isAddingRoad || startRoadPos.isNull()) return;

    QPointF scenePos = view->mapToScene(event->pos());

    if (newRoad) {
        scene->removeItem(newRoad);
        delete newRoad;
        newRoad = nullptr;
    }

    qreal width = qAbs(scenePos.x() - startRoadPos.x());
    qreal height = qAbs(scenePos.y() - startRoadPos.y());

    if (width > height)
        height = 10;
    else
        width = 10;

    newRoad = new Road(roads.size() +1, startRoadPos.x(), startRoadPos.y(), width, height, scene, graph);
    scene->addItem(newRoad);
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (!isAddingRoad || !newRoad) return;

    QPointF scenePos = view->mapToScene(event->pos());

    roads.append(newRoad);

    checkForIntersections(newRoad);

    qDebug() << "Estrada finalizada de" << startRoadPos << "para" << scenePos;

    newRoad = nullptr;
    isAddingRoad = false;
    setCursor(Qt::ArrowCursor);
}
