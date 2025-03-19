#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "road.h"
#include "intersection.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene,this);

    setCentralWidget(view);

    setupScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupScene(){
    scene->setSceneRect (0,0, 800,600);

    QGraphicsRectItem *road = scene -> addRect(100,250,600,100, QPen(Qt::black), QBrush(Qt::darkGray));

    QGraphicsEllipseItem *intersection = scene ->addEllipse(350,250,100,100,QPen(Qt::black),QBrush(Qt::gray));

    new Road (100, 250, 600, 100, scene);
    new Road (350, 50, 100, 500 , scene);

    new Intersection (350, 250,100, scene);
    view->setRenderHint(QPainter::Antialiasing);
}
