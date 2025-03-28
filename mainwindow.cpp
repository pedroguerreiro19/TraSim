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


    Road* r1 = new Road(100, 250, 600, 10, scene);
    Road* r2 = new Road(350, 50, 10, 500, scene);
    roads.append(r1);
    roads.append(r2);


    intersections.append(new Intersection(350, 250, 10, scene));


    // graph[QPointF(100, 250)] = {QPointF(700, 250)};
    // graph[QPointF(350, 50)] = {QPointF(350, 550)};

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
    if (!isAddingRoad) return;

    QPointF scenePos = view->mapToScene(event->pos());

    Road* r1 = roads.first();


    QPointF startPos = r1->pos();
    QPointF endPos = startPos + QPointF(r1->boundingRect().width(), 0);


    qreal distanceStart = QLineF(scenePos, startPos).length();
    qreal distanceEnd = QLineF(scenePos, endPos).length();

    if (distanceStart < 10) {
        startRoadPos = startPos;
        qDebug() << "Clique na extremidade inicial da estrada";
    } else if (distanceEnd < 10) {
        startRoadPos = endPos;
        qDebug() << "Clique na extremidade final da estrada";
    } else {
        return;
    }


    qDebug() << "Adicionando estrada a partir de" << startRoadPos;
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

    newRoad = new Road(startRoadPos.x(), startRoadPos.y(), width, height, scene);
    scene->addItem(newRoad);
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (!isAddingRoad || !newRoad) return;

    QPointF scenePos = view->mapToScene(event->pos());


    roads.append(newRoad);


    //graph[startRoadPos].append(scenePos);
    //graph[scenePos].append(startRoadPos);

    // Verificar e adicionar interseções
    checkForIntersections(newRoad);

    qDebug() << "Estrada finalizada de" << startRoadPos << "para" << scenePos;


    newRoad = nullptr;
    isAddingRoad = false;
    setCursor(Qt::ArrowCursor);
}
