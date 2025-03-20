#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "road.h"
#include "intersection.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug> // TIRAR NO FINAL


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isAddingRoad(false)
    , newRoad(nullptr)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene,this);

    //setCentralWidget(view);

    QPushButton *btnAddRoad = new QPushButton("Adicionar estrada",this);
    connect(btnAddRoad, &QPushButton::clicked, this, &MainWindow::toggleaddRoads);
    mainLayout-> addWidget(view);
    mainLayout-> addWidget(btnAddRoad);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setupScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupScene(){
    scene->setSceneRect (0,0, 800,600);

    //QGraphicsRectItem *road = scene -> addRect(100,250,600,100, QPen(Qt::black), QBrush(Qt::darkGray));


    QGraphicsEllipseItem *intersection = scene ->addEllipse(350,250,100,100,QPen(Qt::black),QBrush(Qt::gray));

    roads.append(new Road(100, 250, 600, 100, scene));
    roads.append(new Road(350, 50, 100, 500, scene));

    //new Road (100, 250, 600, 100, scene);
    //new Road (350, 50, 100, 500 , scene);

    intersections.append(new Intersection (350, 250,100, scene));

    view->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::toggleaddRoads() {
    isAddingRoad = !isAddingRoad;
    if (isAddingRoad){
        qDebug() << "Modo Adicionar Estrada Ativado"; // TIRAR DEPOIS
        setCursor(Qt::CrossCursor);

        QPointF scenePos = view->mapToScene(mapFromGlobal(QCursor::pos()));
        if (!newRoad) {
            newRoad = new Road(scenePos.x(), scenePos.y(), 100, 10, scene);
        }
    } else {
        qDebug() << "Modo Adicionar Estrada Desativado"; // TIRAR DEPOIS
        setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (isAddingRoad && newRoad) {
        QPointF scenePos = view->mapToScene(event->pos());
        newRoad->setPos(scenePos.x(), scenePos.y());
        qDebug() << "Movendo estrada para: " << scenePos; // TIRAR DEPOIS
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if (isAddingRoad) {
        QPointF scenePos = view->mapToScene(event->pos());

        if (newRoad == nullptr) {
            qDebug() << "Criando nova estrada"; // TIRAR DEPOIS
            newRoad = new Road(scenePos.x(), scenePos.y(), 100, 10, scene);
            roads.append(newRoad);
        } else {
            qDebug() <<"Finalizando a estrada";
            //QPointF endPos = view->mapToScene(event->pos());
            //newRoad->setRect(newRoad->x(), newRoad->y(), endPos.x() - newRoad->x(), endPos.y() - newRoad->y());
            newRoad->setFlag(QGraphicsItem::ItemIsMovable, true);
            newRoad = nullptr;
            isAddingRoad = false;
            setCursor(Qt::ArrowCursor);
        }
    }
}


