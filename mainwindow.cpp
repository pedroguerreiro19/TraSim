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
    , newRoad(nullptr)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    QPushButton *btnAddRoad = new QPushButton("Adicionar estrada", this);
    connect(btnAddRoad, &QPushButton::clicked, this, &MainWindow::toggleAddRoads);
    mainLayout->addWidget(view);
    mainLayout->addWidget(btnAddRoad);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setupScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupScene() {
    scene->setSceneRect(0, 0, 800, 600);
    roads.append(new Road(100, 250, 600, 100, scene));
    roads.append(new Road(350, 50, 100, 500, scene));
    intersections.append(new Intersection(350, 250, 100, scene));
    view->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::toggleAddRoads() {
    isAddingRoad = !isAddingRoad;
    setCursor(isAddingRoad ? Qt::CrossCursor : Qt::ArrowCursor);
    qDebug() << (isAddingRoad ? "Modo Adicionar Estrada Ativado" : "Modo Adicionar Estrada Desativado");
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (!isAddingRoad) return;

    QPointF scenePos = view->mapToScene(event->pos());

    // Verificar se o clique foi sobre uma estrada existente
    for (Road* road : roads) {
        if (road->contains(road->mapFromScene(scenePos))) {
            qDebug() << "Iniciando nova estrada a partir de uma existente";

            // Definir ponto de início para a nova estrada
            startRoadPos = scenePos;

            // Criar uma estrada temporária com tamanho mínimo
            newRoad = new Road(scenePos.x(), scenePos.y(), 1, 10, scene);
            scene->addItem(newRoad);

            return;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!newRoad) return;

    QPointF scenePos = view->mapToScene(event->pos());

    // Ajustar largura/altura da estrada enquanto o mouse se move
    qreal width = qAbs(scenePos.x() - startRoadPos.x());
    qreal height = qAbs(scenePos.y() - startRoadPos.y());

    // Manter estrada horizontal ou vertical
    if (width > height)
        height = 10;  // Estrada horizontal
    else
        width = 10;   // Estrada vertical

    newRoad->setRect(QRectF(startRoadPos, QSizeF(width, height)));
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (!newRoad) return;

    qDebug() << "Finalizando estrada";

    // Finalizar e adicionar estrada à lista
    roads.append(newRoad);

    // Verificar interseções e ajustar ligações
    checkForIntersections(newRoad);

    newRoad = nullptr;
    isAddingRoad = false;
    setCursor(Qt::ArrowCursor);
}

void MainWindow::checkForIntersections(Road *road) {
    for (Road *existingRoad : roads) {
        if (existingRoad != road && existingRoad->collidesWithItem(road)) {
            QPointF intersectionPoint = road->boundingRect().center();
            intersections.append(new Intersection(intersectionPoint.x(), intersectionPoint.y(), 20, scene));
        }
    }
}




