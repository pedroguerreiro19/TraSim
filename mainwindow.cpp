#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "carspawner.h"
#include "trafficlight.h"
#include "car.h"
#include "trafficlightgroup.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QMessageBox>


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

    ui->lblSimulationTime->setStyleSheet("font-size: 16px; color: #cccccc;");
    ui->lblSimulationInfo->setStyleSheet("font-size: 18px; font-weight: bold;");
    ui->lblSimulationChanges->setStyleSheet("font-size: 16px;");

    for (auto* btn : {ui->btnShowCharts, ui->btnPauseResumeCars, ui->btnRestartSimulation, ui->btnSpawnDespawn}) {
        btn->setMinimumHeight(36);
    }

    this->showMaximized();

    QTimer* tableUpdateTimer = new QTimer(this);
    connect(tableUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCarDataTable);
    tableUpdateTimer->start(100);

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, [this]() {
        qint64 totalMs = simulationElapsedMs;
        if (simulationRunning) {
            totalMs += elapsedTimer.elapsed();
        }
        int min = (totalMs / 60000);
        int sec = (totalMs / 1000) % 60;
        int ms  = totalMs % 1000;
        ui->lblSimulationTime->setText(
            QString("Simulation time: %1:%2:%3")
                .arg(min, 2, 10, QLatin1Char('0'))
                .arg(sec, 2, 10, QLatin1Char('0'))
                .arg(ms, 3, 10, QLatin1Char('0'))
            );
    });
    simulationTimer->start(50);
    simulationRunning = false;
    simulationElapsedMs = 0;

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


    //Sul Norte
    //faixa esquerda
    graph->addNode(1, QPointF(1711, 1072), NodeType::Spawn);
    graph->addNode(2, QPointF(1711, 972), NodeType::Ordinary);
    graph->addNode(3, QPointF(1711, 872), NodeType::Ordinary);
    graph->addNode(4, QPointF(1711, 772), NodeType::Ordinary);
    graph->addNode(5, QPointF(1711, 672), NodeType::Ordinary);
    graph->addNode(6, QPointF(1711, 572), NodeType::Ordinary);
    graph->addNode(7, QPointF(1711, 472), NodeType::Ordinary);
    graph->addNode(8, QPointF(1711, 372), NodeType::Ordinary);
    graph->addNode(9, QPointF(1711, 272), NodeType::Ordinary);
    graph->addNode(10, QPointF(1711, 204), NodeType::Yield);

    graph->addEdge(1,2,1);
    graph->addEdge(2,3,1);
    graph->addEdge(3,4,1);
    graph->addEdge(4,5,1);
    graph->addEdge(5,6,1);
    graph->addEdge(6,7,1);
    graph->addEdge(7,8,1);
    graph->addEdge(8,9,1);
    graph->addEdge(9,10,1);
    graph->addEdge(3,14,1);
    graph->addEdge(8,19,1);

    //faixa meio
    graph->addNode(11, QPointF(1734, 1072), NodeType::Spawn);
    graph->addNode(12, QPointF(1734, 972), NodeType::Ordinary);
    graph->addNode(13, QPointF(1734, 872), NodeType::Ordinary);
    graph->addNode(14, QPointF(1734, 772), NodeType::Ordinary);
    graph->addNode(15, QPointF(1734, 672), NodeType::Ordinary);
    graph->addNode(16, QPointF(1734, 572), NodeType::Ordinary);
    graph->addNode(17, QPointF(1734, 472), NodeType::Ordinary);
    graph->addNode(28, QPointF(1734, 414), NodeType::Ordinary);
    graph->addNode(18, QPointF(1734, 372), NodeType::Ordinary);
    graph->addNode(19, QPointF(1734, 272), NodeType::Ordinary);
    graph->addNode(20, QPointF(1734, 204), NodeType::Yield);

    graph->addEdge(11,12,1);
    graph->addEdge(12,13,1);
    graph->addEdge(13,14,1);
    graph->addEdge(14,15,1);
    graph->addEdge(15,16,1);
    graph->addEdge(16,17,1);
    graph->addEdge(17,28,1);
    graph->addEdge(28,18,1);
    graph->addEdge(18,19,1);
    graph->addEdge(19,20,1);

    //faixa direita
    graph->addNode(21, QPointF(1750, 1072), NodeType::Spawn);
    graph->addNode(22, QPointF(1750, 972), NodeType::Ordinary);
    graph->addNode(23, QPointF(1750, 872), NodeType::Ordinary);
    graph->addNode(24, QPointF(1750, 772), NodeType::Ordinary);
    graph->addNode(25, QPointF(1750, 672), NodeType::Ordinary);
    graph->addNode(26, QPointF(1750, 572), NodeType::Ordinary);
    graph->addNode(27, QPointF(1750, 457), NodeType::Ordinary);

    graph->addEdge(21,22,1);
    graph->addEdge(22,23,1);
    graph->addEdge(23,24,1);
    graph->addEdge(24,25,1);
    graph->addEdge(25,26,1);
    graph->addEdge(26,27,1);
    graph->addEdge(27,28,1);

    //Norte Sul
    //faixa direita
    graph->addNode(31, QPointF(1691, 204), NodeType::Ordinary);
    graph->addNode(32, QPointF(1691, 272), NodeType::Ordinary);
    graph->addNode(33, QPointF(1691, 372), NodeType::Ordinary);
    graph->addNode(34, QPointF(1691, 472), NodeType::Ordinary);
    graph->addNode(35, QPointF(1691, 572), NodeType::Ordinary);
    graph->addNode(36, QPointF(1691, 672), NodeType::Ordinary);
    graph->addNode(37, QPointF(1691, 772), NodeType::Ordinary);
    graph->addNode(38, QPointF(1691, 872), NodeType::Ordinary);
    graph->addNode(39, QPointF(1691, 972), NodeType::Ordinary);
    graph->addNode(40, QPointF(1691, 1072), NodeType::Despawn);

    graph->addEdge(31,32,1);
    graph->addEdge(32,33,1);
    graph->addEdge(33,34,1);
    graph->addEdge(34,35,1);
    graph->addEdge(35,36,1);
    graph->addEdge(36,37,1);
    graph->addEdge(37,38,1);
    graph->addEdge(38,39,1);
    graph->addEdge(39,40,1);

    //faixa meio
    graph->addNode(41, QPointF(1676, 204), NodeType::Ordinary);
    graph->addNode(42, QPointF(1676, 272), NodeType::Ordinary);
    graph->addNode(43, QPointF(1676, 372), NodeType::Ordinary);
    graph->addNode(51, QPointF(1676, 414), NodeType::Ordinary);
    graph->addNode(44, QPointF(1676, 472), NodeType::Ordinary);
    graph->addNode(45, QPointF(1676, 572), NodeType::Ordinary);
    graph->addNode(46, QPointF(1676, 672), NodeType::Ordinary);
    graph->addNode(47, QPointF(1676, 772), NodeType::Ordinary);
    graph->addNode(48, QPointF(1676, 872), NodeType::Ordinary);
    graph->addNode(49, QPointF(1676, 972), NodeType::Ordinary);
    graph->addNode(50, QPointF(1676, 1072), NodeType::Despawn);

    graph->addEdge(41,42,1);
    graph->addEdge(42,43,1);
    graph->addEdge(43,51,1);
    graph->addEdge(51,44,1);
    graph->addEdge(44,45,1);
    graph->addEdge(45,46,1);
    graph->addEdge(46,47,1);
    graph->addEdge(47,48,1);
    graph->addEdge(48,49,1);
    graph->addEdge(49,50,1);

    //faixa esquerda
    graph->addNode(54, QPointF(1653, 457), NodeType::Ordinary);
    graph->addNode(62, QPointF(1653, 560), NodeType::Ordinary);
    graph->addNode(55, QPointF(1653, 572), NodeType::Ordinary);
    graph->addNode(56, QPointF(1653, 672), NodeType::Ordinary);
    graph->addNode(57, QPointF(1653, 772), NodeType::Ordinary);
    graph->addNode(58, QPointF(1653, 872), NodeType::Ordinary);
    graph->addNode(59, QPointF(1653, 972), NodeType::Ordinary);
    graph->addNode(60, QPointF(1653, 1072), NodeType::Despawn);

    graph->addEdge(51,54,1);
    graph->addEdge(54,62,1);
    graph->addEdge(62,55,1);
    graph->addEdge(55,56,1);
    graph->addEdge(56,57,1);
    graph->addEdge(57,58,1);
    graph->addEdge(58,59,1);
    graph->addEdge(59,60,1);
    //mudanças de faixa
    graph->addEdge(39,3,1);
    graph->addEdge(38,4,1);
    graph->addEdge(37,5,1);
    graph->addEdge(36,6,1);
    graph->addEdge(34,18,1);
    graph->addEdge(18,9,1);
    graph->addEdge(39,3,1);
    graph->addEdge(43,34,1);
    graph->addEdge(49,36,1);
    graph->addEdge(37,48,1);
    graph->addEdge(39,50,1);
    graph->addEdge(59,50,1);
    graph->addEdge(49,40,1);




    //rotunda autoestrada
    graph->addNode(61, QPointF(1708, 160), NodeType::Ordinary);
    graph->addNode(355, QPointF(1700, 65), NodeType::Ordinary);
    graph->addNode(356, QPointF(1652, 107), NodeType::Ordinary);
    graph->addNode(357, QPointF(1750, 112), NodeType::Ordinary);
    graph->addNode(358, QPointF(1749, 183), NodeType::Ordinary);
    graph->addNode(359, QPointF(1788, 145), NodeType::Ordinary);
    graph->addNode(360, QPointF(1785, 82), NodeType::Yield);
    graph->addNode(361, QPointF(1767, 50), NodeType::Ordinary);
    graph->addNode(362, QPointF(1738, 29), NodeType::Ordinary);
    graph->addNode(363, QPointF(1681, 27), NodeType::Yield);
    graph->addNode(364, QPointF(1643, 45), NodeType::Ordinary);
    graph->addNode(365, QPointF(1623, 82), NodeType::Ordinary);
    graph->addNode(366, QPointF(1623, 104), NodeType::Ordinary);
    graph->addNode(367, QPointF(1625, 138), NodeType::Yield);
    graph->addNode(368, QPointF(1641, 178), NodeType::Ordinary);
    graph->addNode(373, QPointF(1739, 82), NodeType::Ordinary);
    graph->addNode(374, QPointF(1670, 79), NodeType::Ordinary);
    graph->addNode(375, QPointF(1668, 141), NodeType::Ordinary);
    graph->addNode(376, QPointF(1736, 144), NodeType::Ordinary);
    //saida direita
    graph->addNode(369, QPointF(1913, 145), NodeType::Despawn);
    graph->addNode(370, QPointF(1910, 103), NodeType::Spawn);
    //saida norte
    graph->addNode(371, QPointF(1733, 3), NodeType::Despawn);
    graph->addNode(372, QPointF(1677, 3), NodeType::Spawn);



    graph->addEdge(10,61,1);
    graph->addEdge(61,357,1);
    graph->addEdge(61,376,1);
    graph->addEdge(376,357,1);
    graph->addEdge(61,358,1);
    graph->addEdge(357,373,1);
    graph->addEdge(373,355,1);
    graph->addEdge(355,364,1);
    graph->addEdge(355,374,1);
    graph->addEdge(374,356,1);
    graph->addEdge(356,368,1);
    graph->addEdge(368,41,1);
    graph->addEdge(356,375,1);
    graph->addEdge(357,375,1);
    graph->addEdge(375,61,1);
    graph->addEdge(357,361,1);
    graph->addEdge(362,363,1);
    graph->addEdge(20,358,1);
    graph->addEdge(358,359,1);
    graph->addEdge(359,369,1);
    graph->addEdge(370,360,1);
    graph->addEdge(370,373,1);
    graph->addEdge(360,361,1);
    graph->addEdge(360,373,1);
    graph->addEdge(360,361,1);
    graph->addEdge(361,362,1);
    graph->addEdge(362,363,1);
    graph->addEdge(362,371,1);
    graph->addEdge(372,363,1);
    graph->addEdge(372,374,1);
    graph->addEdge(363,364,1);
    graph->addEdge(364,365,1);
    graph->addEdge(365,412,1);

    //Rua principal
    graph->addNode(63, QPointF(1638, 590), NodeType::Ordinary);
    graph->addNode(64, QPointF(1641, 672), NodeType::Ordinary);
    graph->addNode(65, QPointF(1628, 694), NodeType::Ordinary);
    graph->addNode(66, QPointF(1609, 708), NodeType::Ordinary);
    graph->addNode(67, QPointF(1437, 711), NodeType::Ordinary);
    //Virar a primeira direita
    graph->addNode(109, QPointF(1416, 690), NodeType::Ordinary);
    graph->addNode(110, QPointF(1419, 428), NodeType::Despawn);
    graph->addNode(111, QPointF(1398, 427), NodeType::Spawn);
    graph->addNode(112, QPointF(1398, 688), NodeType::Yield);
    graph->addEdge(67,109,1);
    graph->addEdge(109,110,1);
    graph->addEdge(111,112,1);
    graph->addEdge(112,68,1);
    graph->addEdge(112,104,1);
    graph->addEdge(112,115,1);
    ////////////////////////////
    //Virar a primeira esquerda
    graph->addNode(113, QPointF(1395, 427), NodeType::Ordinary);
    graph->addNode(114, QPointF(1419, 735), NodeType::Yield);
    graph->addNode(115, QPointF(1395, 735), NodeType::Ordinary);
    graph->addNode(150, QPointF(1396, 1018), NodeType::Despawn);
    graph->addNode(151, QPointF(1416, 1019), NodeType::Spawn);
    graph->addEdge(67,115,1);
    graph->addEdge(115,150,1);
    graph->addEdge(151,114,1);
    graph->addEdge(114,68,1);
    graph->addEdge(114,104,1);
    graph->addEdge(114,109,1);
    ////////////////////////////
    graph->addNode(68, QPointF(1383, 711), NodeType::Ordinary);
    graph->addNode(69, QPointF(1213, 711), NodeType::Light);
    //Virar a segunda direita
    graph->addNode(116, QPointF(1193, 685), NodeType::Ordinary);
    graph->addNode(117, QPointF(1186, 439), NodeType::Ordinary);
    graph->addNode(118, QPointF(1178, 422), NodeType::Ordinary);
    graph->addNode(119, QPointF(1153, 410), NodeType::Ordinary);
    graph->addNode(120, QPointF(991, 405), NodeType::Ordinary);
    graph->addNode(121, QPointF(967, 419), NodeType::Ordinary);
    graph->addNode(122, QPointF(951, 440), NodeType::Ordinary);
    graph->addNode(123, QPointF(948, 535), NodeType::Ordinary);
    //Virar rotunda pequena
    graph->addNode(141, QPointF(935, 551), NodeType::Ordinary);
    graph->addNode(142, QPointF(947, 579), NodeType::Ordinary);
    graph->addEdge(123,141,1);
    graph->addEdge(141,193,1);
    graph->addEdge(193,194,1);
    graph->addEdge(193,205,1);
    graph->addEdge(126,141,1);
    graph->addEdge(377,127,1);
    graph->addEdge(377,142,1);
    graph->addEdge(142,124,1);

    graph->addEdge(69,116,1);
    graph->addEdge(116,117,1);
    graph->addEdge(117,118,1);
    graph->addEdge(118,119,1);
    graph->addEdge(119,120,1);
    graph->addEdge(120,121,1);
    graph->addEdge(121,122,1);
    graph->addEdge(122,123,1);
    graph->addEdge(123,124,1);
    //Virar para a rotunda
    //
    graph->addNode(124, QPointF(950, 678), NodeType::Ordinary);
    ////////////////////////
    //Virar a segunda esquerda
    graph->addEdge(69,85,1);
    ////////////////////////

    graph->addNode(70, QPointF(1156, 711), NodeType::Ordinary);
    graph->addNode(71, QPointF(986, 711), NodeType::Ordinary);
    //Virar a terceira direita
    graph->addNode(125, QPointF(967, 678), NodeType::Ordinary);
    graph->addNode(126, QPointF(965, 578), NodeType::Ordinary);
    graph->addNode(127, QPointF(967, 538), NodeType::Ordinary);
    graph->addNode(128, QPointF(965, 446), NodeType::Ordinary);
    graph->addNode(129, QPointF(974, 430), NodeType::Ordinary);
    graph->addNode(130, QPointF(995, 428), NodeType::Ordinary);
    graph->addNode(131, QPointF(1141, 425), NodeType::Ordinary);
    graph->addNode(132, QPointF(1166, 434), NodeType::Ordinary);
    graph->addNode(133, QPointF(1172, 455), NodeType::Ordinary);
    graph->addNode(134, QPointF(1172, 682), NodeType::Ordinary);

    graph->addEdge(71,125,1);
    graph->addEdge(125,126,1);
    graph->addEdge(126,127,1);
    graph->addEdge(127,128,1);
    graph->addEdge(128,129,1);
    graph->addEdge(129,130,1);
    graph->addEdge(130,131,1);
    graph->addEdge(131,132,1);
    graph->addEdge(132,133,1);
    graph->addEdge(133,134,1);
    graph->addEdge(134,85,1);
    graph->addEdge(134,70,1);
    graph->addEdge(134,102,1);
    //////////////////////////////////
    //Virar a terceira esquerda
    graph->addNode(135, QPointF(968, 746), NodeType::Ordinary);
    graph->addNode(136, QPointF(967, 912), NodeType::Ordinary);
    graph->addNode(137, QPointF(944, 911), NodeType::Ordinary);
    graph->addNode(138, QPointF(947, 744), NodeType::Ordinary);
    graph->addEdge(71,138,1);
    graph->addEdge(138,137,1);
    graph->addEdge(137,90,1);
    graph->addEdge(137,80,1);
    graph->addEdge(89,136,1);
    graph->addEdge(136,135,1);
    graph->addEdge(135,100,1);
    graph->addEdge(135,125,1);
    graph->addEdge(135,72,1);
    //////////////////////////////

    graph->addNode(72, QPointF(932, 711), NodeType::Ordinary);
    graph->addNode(73, QPointF(820, 711), NodeType::Ordinary);
    //Virar a quarta direita / rotunda
    graph->addNode(139, QPointF(817, 626), NodeType::Yield);
    graph->addNode(140, QPointF(788, 711), NodeType::Ordinary);
    graph->addEdge(73,139,1);
    graph->addEdge(139,190,1);
    graph->addEdge(190,191,1);
    graph->addEdge(139,203,1);
    graph->addEdge(201,378,1);
    graph->addEdge(378,140,1);
    graph->addEdge(378,98,1);
    graph->addEdge(140,74,1);
    graph->addEdge(72,140,1);
    //////////////////////////
    graph->addNode(74, QPointF(664, 717), NodeType::Ordinary);
    graph->addNode(75, QPointF(649, 740), NodeType::Ordinary);
    graph->addNode(76, QPointF(653, 906), NodeType::Ordinary);
    graph->addNode(77, QPointF(664, 929), NodeType::Ordinary);
    graph->addNode(78, QPointF(675, 942), NodeType::Ordinary);
    graph->addNode(79, QPointF(930, 947), NodeType::Ordinary);
    graph->addNode(80, QPointF(986, 947), NodeType::Ordinary);
    graph->addNode(81, QPointF(1159, 950), NodeType::Ordinary);
    graph->addNode(82, QPointF(1178, 936), NodeType::Ordinary);
    graph->addNode(83, QPointF(1184, 917), NodeType::Ordinary);
    graph->addNode(84, QPointF(1192, 746), NodeType::Ordinary);

    graph->addEdge(62,63,1);
    graph->addEdge(63,64,1);
    graph->addEdge(64,65,1);
    graph->addEdge(64,65,1);
    graph->addEdge(65,66,1);
    graph->addEdge(66,67,1);
    graph->addEdge(67,68,1);
    graph->addEdge(68,69,1);
    graph->addEdge(69,70,1);
    graph->addEdge(70,71,1);
    graph->addEdge(71,72,1);
    graph->addEdge(72,74,1);
    graph->addEdge(74,75,1);
    graph->addEdge(75,76,1);
    graph->addEdge(76,77,1);
    graph->addEdge(77,78,1);
    graph->addEdge(78,79,1);
    graph->addEdge(79,136,1);
    graph->addEdge(79,80,1);
    graph->addEdge(80,81,1);
    graph->addEdge(81,82,1);
    graph->addEdge(82,83,1);
    graph->addEdge(83,84,1);
    graph->addEdge(84,70,1);
    graph->addEdge(84,116,1);
    graph->addEdge(84,102,1);

    //Rua principal direçao contraria
    graph->addNode(85, QPointF(1171, 744), NodeType::Ordinary);
    graph->addNode(86, QPointF(1172, 906), NodeType::Ordinary);
    graph->addNode(87, QPointF(1169, 921), NodeType::Ordinary);
    graph->addNode(88, QPointF(1150, 932), NodeType::Ordinary);
    graph->addNode(89, QPointF(985, 926), NodeType::Ordinary);
    graph->addNode(90, QPointF(929, 926), NodeType::Ordinary);
    graph->addNode(91, QPointF(679, 929), NodeType::Ordinary);
    graph->addNode(92, QPointF(673, 917), NodeType::Ordinary);
    graph->addNode(93, QPointF(670, 895), NodeType::Ordinary);
    graph->addNode(94, QPointF(670, 741), NodeType::Ordinary);
    graph->addNode(95, QPointF(676, 729), NodeType::Ordinary);
    graph->addNode(96, QPointF(790, 728), NodeType::Ordinary);
    graph->addNode(97, QPointF(779, 728), NodeType::Ordinary);
    graph->addNode(98, QPointF(833, 728), NodeType::Ordinary);
    graph->addNode(99, QPointF(926, 728), NodeType::Ordinary);
    graph->addNode(100, QPointF(986, 728), NodeType::Ordinary);
    graph->addNode(101, QPointF(1153, 728), NodeType::Light);
    graph->addNode(102, QPointF(1209, 728), NodeType::Ordinary);
    graph->addNode(103, QPointF(1379, 728), NodeType::Ordinary);
    graph->addNode(104, QPointF(1431, 728), NodeType::Ordinary);
    graph->addNode(105, QPointF(1584, 728), NodeType::Ordinary);
    graph->addNode(106, QPointF(1614, 728), NodeType::Ordinary);
    graph->addNode(107, QPointF(1641, 756), NodeType::Ordinary);
    graph->addNode(108, QPointF(1634, 818), NodeType::Ordinary);
    graph->addNode(510, QPointF(820, 688), NodeType::Ordinary);

    graph->addEdge(85,86,1);
    graph->addEdge(86,87,1);
    graph->addEdge(87,88,1);
    graph->addEdge(88,89,1);
    graph->addEdge(89,90,1);
    graph->addEdge(90,91,1);
    graph->addEdge(91,92,1);
    graph->addEdge(92,93,1);
    graph->addEdge(93,94,1);
    graph->addEdge(94,95,1);
    graph->addEdge(95,97,1);
    graph->addEdge(97,96,1);
    graph->addEdge(96,98,1);
    graph->addEdge(96,73,1);
    graph->addEdge(96,510,1);
    graph->addEdge(510,139,1);

    graph->addEdge(98,99,1);
    graph->addEdge(99,100,1);
    graph->addEdge(100,101,1);
    graph->addEdge(101,102,1);
    graph->addEdge(102,103,1);
    graph->addEdge(103,104,1);
    graph->addEdge(103,109,1);
    graph->addEdge(103,115,1);
    graph->addEdge(104,105,1);
    graph->addEdge(105,106,1);
    graph->addEdge(106,107,1);
    graph->addEdge(107,108,1);
    graph->addEdge(108,58,1);




    //rotunda pequena
    graph->addNode(190, QPointF(826, 613), NodeType::Ordinary);
    graph->addNode(191, QPointF(847, 601), NodeType::Ordinary);
    graph->addNode(192, QPointF(862, 578), NodeType::Ordinary);
    graph->addNode(193, QPointF(864, 551), NodeType::Yield);
    graph->addNode(194, QPointF(841, 529), NodeType::Ordinary);
    graph->addNode(195, QPointF(820, 511), NodeType::Ordinary);
    graph->addNode(196, QPointF(797, 511), NodeType::Yield);
    graph->addNode(197, QPointF(776, 531), NodeType::Ordinary);
    graph->addNode(198, QPointF(756, 555), NodeType::Ordinary);
    graph->addNode(199, QPointF(755, 576), NodeType::Yield);
    graph->addNode(200, QPointF(768, 601), NodeType::Ordinary);
    graph->addNode(201, QPointF(799, 617), NodeType::Ordinary);
    graph->addNode(202, QPointF(815, 593), NodeType::Ordinary);
    graph->addNode(203, QPointF(830, 581), NodeType::Ordinary);
    graph->addNode(204, QPointF(836, 567), NodeType::Ordinary);
    graph->addNode(205, QPointF(829, 546), NodeType::Ordinary);
    graph->addNode(206, QPointF(806, 540), NodeType::Ordinary);
    graph->addNode(207, QPointF(785, 546), NodeType::Ordinary);
    graph->addNode(208, QPointF(784, 566), NodeType::Ordinary);
    graph->addNode(209, QPointF(790, 585), NodeType::Ordinary);
    //saida 1
    graph->addNode(377, QPointF(927, 575), NodeType::Yield);
    //saida 4
    graph->addNode(378, QPointF(797, 688), NodeType::Yield);



    //entrada autoestrada
    graph->addNode(352, QPointF(824, 201), NodeType::Ordinary);
    graph->addNode(550, QPointF(852, 183), NodeType::Ordinary);
    graph->addNode(353, QPointF(898, 183), NodeType::Ordinary);
    graph->addNode(354, QPointF(968, 172), NodeType::Ordinary);


    graph->addNode(210, QPointF(666, 552), NodeType::Ordinary);
    graph->addNode(211, QPointF(664, 545), NodeType::Despawn);
    graph->addNode(213, QPointF(652, 546), NodeType::Spawn);
    graph->addNode(214, QPointF(661, 573), NodeType::Ordinary);

    graph->addEdge(196,207,1);
    graph->addEdge(207,208,1);
    graph->addEdge(208,200,1);
    graph->addEdge(208,209,1);
    graph->addEdge(209,202,1);
    graph->addEdge(202,191,1);
    graph->addEdge(191,192,1);
    graph->addEdge(192,377,1);
    graph->addEdge(202,203,1);
    graph->addEdge(203,204,1);
    graph->addEdge(204,205,1);
    graph->addEdge(205,206,1);
    graph->addEdge(206,207,1);
    graph->addEdge(206,197,1);
    graph->addEdge(204,194,1);
    graph->addEdge(194,195,1);
    graph->addEdge(195,352,1);
    graph->addEdge(352,550,1);
    graph->addEdge(550,353,1);
    graph->addEdge(353,354,1);
    graph->addEdge(354,344,1);
    graph->addEdge(200,201,1);
    graph->addEdge(201,378,1);
    graph->addEdge(196,197,1);
    graph->addEdge(197,198,1);
    graph->addEdge(198,210,1);
    graph->addEdge(210,211,1);
    graph->addEdge(213,214,1);
    graph->addEdge(214,199,1);
    graph->addEdge(199,200,1);
    graph->addEdge(199,209,1);




    //Oeste Este
    //faixa esquerda
    graph->addNode(300, QPointF(0, 121), NodeType::Spawn);
    graph->addNode(301, QPointF(100, 121), NodeType::Ordinary);
    graph->addNode(302, QPointF(200, 121), NodeType::Ordinary);
    graph->addNode(303, QPointF(300, 121), NodeType::Ordinary);
    graph->addNode(304, QPointF(400, 121), NodeType::Ordinary);
    graph->addNode(305, QPointF(500, 121), NodeType::Ordinary);
    graph->addNode(306, QPointF(600, 121), NodeType::Ordinary);
    graph->addNode(307, QPointF(700, 121), NodeType::Ordinary);
    graph->addNode(308, QPointF(800, 121), NodeType::Ordinary);
    graph->addNode(309, QPointF(900, 121), NodeType::Ordinary);
    graph->addNode(310, QPointF(1000, 121), NodeType::Ordinary);
    graph->addNode(311, QPointF(1100, 121), NodeType::Ordinary);
    graph->addNode(312, QPointF(1200, 121), NodeType::Ordinary);
    graph->addNode(313, QPointF(1300, 121), NodeType::Ordinary);
    graph->addNode(314, QPointF(1400, 121), NodeType::Ordinary);
    graph->addNode(315, QPointF(1500, 121), NodeType::Ordinary);
    graph->addNode(316, QPointF(1614, 121), NodeType::Yield);

    graph->addEdge(300,301,1);
    graph->addEdge(301,302,1);
    graph->addEdge(302,303,1);
    graph->addEdge(303,304,1);
    graph->addEdge(304,305,1);
    graph->addEdge(305,306,1);
    graph->addEdge(306,307,1);
    graph->addEdge(307,308,1);
    graph->addEdge(308,309,1);
    graph->addEdge(309,310,1);
    graph->addEdge(310,311,1);
    graph->addEdge(311,312,1);
    graph->addEdge(312,313,1);
    graph->addEdge(313,314,1);
    graph->addEdge(314,315,1);
    graph->addEdge(315,316,1);
    graph->addEdge(315,333,1);
    graph->addEdge(316,375,1);
    //faixa meio
    graph->addNode(317, QPointF(0, 141), NodeType::Spawn);
    graph->addNode(318, QPointF(100, 141), NodeType::Ordinary);
    graph->addNode(319, QPointF(200, 141), NodeType::Ordinary);
    graph->addNode(320, QPointF(300, 141), NodeType::Ordinary);
    graph->addNode(321, QPointF(400, 141), NodeType::Ordinary);
    graph->addNode(322, QPointF(500, 141), NodeType::Ordinary);
    graph->addNode(323, QPointF(600, 141), NodeType::Ordinary);
    graph->addNode(324, QPointF(700, 141), NodeType::Ordinary);
    graph->addNode(325, QPointF(800, 141), NodeType::Ordinary);
    graph->addNode(326, QPointF(900, 141), NodeType::Ordinary);
    graph->addNode(327, QPointF(1000, 141), NodeType::Ordinary);
    graph->addNode(328, QPointF(1100, 141), NodeType::Ordinary);
    graph->addNode(329, QPointF(1200, 141), NodeType::Ordinary);
    graph->addNode(330, QPointF(1300, 141), NodeType::Ordinary);
    graph->addNode(331, QPointF(1400, 141), NodeType::Ordinary);
    graph->addNode(332, QPointF(1500, 141), NodeType::Ordinary);
    graph->addNode(333, QPointF(1614, 141), NodeType::Ordinary);

    graph->addEdge(317,318,1);
    graph->addEdge(318,319,1);
    graph->addEdge(319,320,1);
    graph->addEdge(320,321,1);
    graph->addEdge(321,322,1);
    graph->addEdge(322,323,1);
    graph->addEdge(323,324,1);
    graph->addEdge(324,325,1);
    graph->addEdge(324,308,1);
    graph->addEdge(325,326,1);
    graph->addEdge(326,327,1);
    graph->addEdge(327,328,1);
    graph->addEdge(328,329,1);
    graph->addEdge(329,330,1);
    graph->addEdge(330,331,1);
    graph->addEdge(331,332,1);
    graph->addEdge(332,333,1);
    graph->addEdge(333,367,1);
    graph->addEdge(367,368,1);


    //faixa direita
    graph->addNode(334, QPointF(0, 160), NodeType::Spawn);
    graph->addNode(335, QPointF(100, 160), NodeType::Ordinary);
    graph->addNode(336, QPointF(200, 160), NodeType::Ordinary);
    graph->addNode(337, QPointF(300, 160), NodeType::Ordinary);
    graph->addNode(338, QPointF(400, 160), NodeType::Ordinary);
    graph->addNode(339, QPointF(500, 160), NodeType::Ordinary);
    graph->addNode(340, QPointF(641, 160), NodeType::Ordinary);
    graph->addNode(341, QPointF(700, 160), NodeType::Ordinary);
    graph->addNode(342, QPointF(800, 160), NodeType::Ordinary);
    graph->addNode(343, QPointF(900, 160), NodeType::Ordinary);
    graph->addNode(344, QPointF(1000, 160), NodeType::Ordinary);
    graph->addNode(345, QPointF(1100, 160), NodeType::Ordinary);
    graph->addNode(346, QPointF(1200, 160), NodeType::Ordinary);
    graph->addNode(347, QPointF(1300, 160), NodeType::Ordinary);
    graph->addNode(348, QPointF(1370, 160), NodeType::Ordinary);

    graph->addEdge(334,335,1);
    graph->addEdge(335,336,1);
    graph->addEdge(336,337,1);
    graph->addEdge(337,338,1);
    graph->addEdge(338,339,1);
    graph->addEdge(339,340,1);
    graph->addEdge(340,341,1);
    graph->addEdge(341,342,1);
    graph->addEdge(342,343,1);
    graph->addEdge(343,344,1);
    graph->addEdge(344,345,1);
    graph->addEdge(345,346,1);
    graph->addEdge(346,347,1);
    graph->addEdge(347,348,1);
    graph->addEdge(348,331,1);




    //junção cidade
    graph->addNode(349, QPointF(664, 177), NodeType::Ordinary);
    graph->addNode(350, QPointF(771, 182), NodeType::Ordinary);
    graph->addNode(351, QPointF(797, 215), NodeType::Ordinary);

    graph->addEdge(340,349,1);
    graph->addEdge(349,350,1);
    graph->addEdge(350,351,1);
    graph->addEdge(351,196,1);

    graph->addEdge(212,344,1);
    //Oeste Este
    //faixa esquerda
    graph->addNode(379, QPointF(0, 106), NodeType::Despawn);
    graph->addNode(380, QPointF(100, 106), NodeType::Ordinary);
    graph->addNode(381, QPointF(200, 106), NodeType::Ordinary);
    graph->addNode(382, QPointF(300, 106), NodeType::Ordinary);
    graph->addNode(383, QPointF(400, 106), NodeType::Ordinary);
    graph->addNode(384, QPointF(500, 106), NodeType::Ordinary);
    graph->addNode(385, QPointF(600, 106), NodeType::Ordinary);
    graph->addNode(386, QPointF(700, 106), NodeType::Ordinary);
    graph->addNode(387, QPointF(800, 106), NodeType::Ordinary);
    graph->addNode(388, QPointF(900, 106), NodeType::Ordinary);
    graph->addNode(389, QPointF(1000, 106), NodeType::Ordinary);
    graph->addNode(390, QPointF(1100, 106), NodeType::Ordinary);
    graph->addNode(391, QPointF(1200, 106), NodeType::Ordinary);
    graph->addNode(392, QPointF(1300, 106), NodeType::Ordinary);
    graph->addNode(393, QPointF(1400, 106), NodeType::Ordinary);
    graph->addNode(394, QPointF(1500, 106), NodeType::Ordinary);
    graph->addNode(395, QPointF(1614, 106), NodeType::Ordinary);

    graph->addEdge(380,379,1);
    graph->addEdge(381,380,1);
    graph->addEdge(382,381,1);
    graph->addEdge(383,382,1);
    graph->addEdge(384,383,1);
    graph->addEdge(385,384,1);
    graph->addEdge(386,385,1);
    graph->addEdge(387,386,1);
    graph->addEdge(388,387,1);
    graph->addEdge(389,388,1);
    graph->addEdge(390,389,1);
    graph->addEdge(391,390,1);
    graph->addEdge(392,391,1);
    graph->addEdge(393,392,1);
    graph->addEdge(394,393,1);
    graph->addEdge(395,394,1);
    graph->addEdge(366,395,1);

    //faixa meio
    graph->addNode(396, QPointF(0, 86), NodeType::Despawn);
    graph->addNode(397, QPointF(100, 86), NodeType::Ordinary);
    graph->addNode(398, QPointF(200, 86), NodeType::Ordinary);
    graph->addNode(399, QPointF(300, 86), NodeType::Ordinary);
    graph->addNode(400, QPointF(400, 86), NodeType::Ordinary);
    graph->addNode(401, QPointF(500, 86), NodeType::Ordinary);
    graph->addNode(402, QPointF(600, 86), NodeType::Ordinary);
    graph->addNode(403, QPointF(700, 86), NodeType::Ordinary);
    graph->addNode(404, QPointF(800, 86), NodeType::Ordinary);
    graph->addNode(405, QPointF(900, 86), NodeType::Ordinary);
    graph->addNode(406, QPointF(1000, 86), NodeType::Ordinary);
    graph->addNode(407, QPointF(1100, 86), NodeType::Ordinary);
    graph->addNode(408, QPointF(1200, 86), NodeType::Ordinary);
    graph->addNode(409, QPointF(1300, 86), NodeType::Ordinary);
    graph->addNode(410, QPointF(1400, 86), NodeType::Ordinary);
    graph->addNode(411, QPointF(1500, 86), NodeType::Ordinary);
    graph->addNode(412, QPointF(1614, 86), NodeType::Ordinary);

    graph->addEdge(397,396,1);
    graph->addEdge(398,397,1);
    graph->addEdge(399,398,1);
    graph->addEdge(400,399,1);
    graph->addEdge(401,400,1);
    graph->addEdge(402,401,1);
    graph->addEdge(403,402,1);
    graph->addEdge(404,403,1);
    graph->addEdge(405,404,1);
    graph->addEdge(406,405,1);
    graph->addEdge(407,406,1);
    graph->addEdge(408,407,1);
    graph->addEdge(409,408,1);
    graph->addEdge(410,409,1);
    graph->addEdge(411,410,1);
    graph->addEdge(412,411,1);

    //faixa direita
    graph->addNode(413, QPointF(0, 67), NodeType::Despawn);
    graph->addNode(414, QPointF(100, 67), NodeType::Ordinary);
    graph->addNode(415, QPointF(200, 67), NodeType::Ordinary);
    graph->addNode(416, QPointF(300, 67), NodeType::Ordinary);
    graph->addNode(417, QPointF(400, 67), NodeType::Ordinary);
    graph->addNode(418, QPointF(500, 67), NodeType::Ordinary);
    graph->addNode(419, QPointF(600, 67), NodeType::Ordinary);
    graph->addNode(420, QPointF(700, 67), NodeType::Ordinary);
    graph->addNode(421, QPointF(800, 67), NodeType::Ordinary);
    graph->addNode(422, QPointF(900, 67), NodeType::Ordinary);
    graph->addNode(423, QPointF(1000, 67), NodeType::Ordinary);
    graph->addNode(424, QPointF(1100, 67), NodeType::Ordinary);
    graph->addNode(425, QPointF(1200, 67), NodeType::Ordinary);
    graph->addNode(426, QPointF(1300, 67), NodeType::Ordinary);
    graph->addNode(427, QPointF(1352, 67), NodeType::Ordinary);

    graph->addEdge(414,413,1);
    graph->addEdge(415,414,1);
    graph->addEdge(416,415,1);
    graph->addEdge(417,416,1);
    graph->addEdge(418,417,1);
    graph->addEdge(419,418,1);
    graph->addEdge(420,419,1);
    graph->addEdge(421,420,1);
    graph->addEdge(422,421,1);
    graph->addEdge(423,422,1);
    graph->addEdge(424,423,1);
    graph->addEdge(425,424,1);
    graph->addEdge(426,425,1);
    graph->addEdge(427,426,1);
    graph->addEdge(410,427,1);
    //mudanças de faixa
    graph->addEdge(335,319,1);
    graph->addEdge(339,323,1);
    graph->addEdge(320,304,1);
    graph->addEdge(320,338,1);
    graph->addEdge(329,313,1);
    graph->addEdge(331,315,1);
    graph->addEdge(301,319,1);
    graph->addEdge(309,327,1);
    graph->addEdge(345,329,1);
    graph->addEdge(314,332,1);
    graph->addEdge(392,408,1);
    graph->addEdge(405,421,1);
    graph->addEdge(405,387,1);
    graph->addEdge(385,401,1);
    graph->addEdge(415,397,1);
    graph->addEdge(382,398,1);

    //Despawn e Spawn Extras
    graph->addNode(500, QPointF(640, 803), NodeType::Despawn);
    graph->addNode(501, QPointF(653, 803), NodeType::Spawn);
    graph->addEdge(75,500,1);
    graph->addEdge(501,76,1);
    graph->addNode(502, QPointF(868, 924), NodeType::Despawn);
    graph->addNode(503, QPointF(868, 926), NodeType::Spawn);
    graph->addEdge(90,502,1);
    graph->addEdge(503,91,1);
    graph->addNode(504, QPointF(1165, 650), NodeType::Despawn);
    graph->addNode(505, QPointF(1172, 650), NodeType::Spawn);
    graph->addEdge(133,504,1);
    graph->addEdge(505,134,1);


    Node* node101 = graph->getNode(101);
    if (node101) {
        TrafficLight* light101 = new TrafficLight(node101->position.x() - 5, node101->position.y() + 10, graph, node101, scene);
        light101 -> setRotation(90);
        graph->addTrafficLight(101, light101);
    }

    Node* node69 = graph->getNode(69);
    if (node69) {
        TrafficLight* light69 = new TrafficLight(node69->position.x() + 5, node69->position.y() - 5, graph, node69, scene);
        light69 -> setRotation(-90);
        graph->addTrafficLight(69, light69);
    }

    Node* node71 = graph->getNode(71);
    if (node71) {
        TrafficLight* light71 = new TrafficLight(node71->position.x() + 5, node71->position.y() -5, graph, node71, scene);
        light71 -> setRotation(-90);
        graph->addTrafficLight(71, light71);
    }

    Node* node99 = graph->getNode(99);
    if (node99) {
        TrafficLight* light99 = new TrafficLight(node99->position.x() - 5, node99->position.y() + 10, graph, node99, scene);
        light99 -> setRotation(90);
        graph->addTrafficLight(99, light99);
    }

    Node* node124 = graph->getNode(124);
    if (node124) {
        TrafficLight* light124 = new TrafficLight(node124->position.x() - 10, node124->position.y(), graph, node124, scene);
        light124 -> setRotation(180);
        graph->addTrafficLight(124, light124);
    }

    Node* node135 = graph->getNode(135);
    if (node135) {
        TrafficLight* light135 = new TrafficLight(node135->position.x() + 10, node135->position.y(), graph, node135, scene);
        light135 -> setRotation(0);
        graph->addTrafficLight(135, light135);
    }

    Node* node84 = graph->getNode(84);
    if (node84) {
        TrafficLight* light84 = new TrafficLight(node84->position.x() + 10, node84->position.y(), graph, node84, scene);
        light84 -> setRotation(0);
        graph->addTrafficLight(84, light84);
    }

    Node* node134 = graph->getNode(134);
    if (node134) {
        TrafficLight* light134 = new TrafficLight(node134->position.x() - 10, node134->position.y(), graph, node134, scene);
        light134 -> setRotation(180);
        graph->addTrafficLight(134, light134);
    }

    groupA = new TrafficLightGroup(this);
    groupB = new TrafficLightGroup(this);

    groupA->addTrafficLight(graph->getTrafficLightAtNode(101));
    groupA->addTrafficLight(graph->getTrafficLightAtNode(69));
    groupA->addTrafficLight(graph->getTrafficLightAtNode(71));
    groupA->addTrafficLight(graph->getTrafficLightAtNode(99));

    groupB->addTrafficLight(graph->getTrafficLightAtNode(124));
    groupB->addTrafficLight(graph->getTrafficLightAtNode(134));
    groupB->addTrafficLight(graph->getTrafficLightAtNode(135));
    groupB->addTrafficLight(graph->getTrafficLightAtNode(84));

    groupA->setOpposingGroup(groupB);
    groupA->setAsPrimary(true);
    groupA->startCycle();

    int id = 1;
    for (Node* node : graph->spawnNodes) {
        CarSpawner* spawner = new CarSpawner(id++, graph, scene);
        carSpawners.append(spawner);
    }

    auto addYieldTriangle = [&](int nodeId, qreal offsetX, qreal offsetY, qreal rotation) {
        Node* node = graph->getNode(nodeId);
        if (node) {
            QPolygonF triangle;
            triangle << QPointF(0, 0) << QPointF(20, 0) << QPointF(10, 20);
            QGraphicsPolygonItem* triangleItem = scene->addPolygon(triangle, QPen(Qt::red, 2), QBrush(Qt::white));
            triangleItem->setPos(node->position + QPointF(offsetX, offsetY));
            triangleItem->setRotation(rotation);
            triangleItem->setZValue(1);
        }
    };

    addYieldTriangle(20,  40, 15, 180);
    addYieldTriangle(360, 25, -20, 90);
    addYieldTriangle(363, -30, -15, 0);
    addYieldTriangle(367, -25, 35, -90);
    addYieldTriangle(112, -30, -25, 0);
    addYieldTriangle(114,  25,  25, 180);
    addYieldTriangle(139,  30, 15, 180);
    addYieldTriangle(193,  20,  -20, 90);
    addYieldTriangle(196, -30,  -5, 0);
    addYieldTriangle(199,  -10, 35, -90);
    addYieldTriangle(377, -5,  30, -90);

    setupCarDataTableStyle();
    scene->update();
    visualizarNodes();


    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::visualizarNodes() {
    for (auto node : graph->nodes.values()) {
        QGraphicsEllipseItem* circle = scene->addEllipse(
            node->position.x() - 5, node->position.y() - 5, 10, 10,
            QPen(Qt::blue), QBrush(Qt::cyan));

        QGraphicsTextItem* label = scene->addText(QString::number(node->id));
        label->setPos(node->position.x() + 6, node->position.y() - 6);
        label->setDefaultTextColor(Qt::black);
    }
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

        if (!simulationRunning) {
            simulationRunning = true;
            elapsedTimer.restart();
        }
    } else {
        spawnTimer->stop();
        ui->btnSpawnDespawn->setText("Start vehicle spawning");
        spawning = false;
    }
}
void MainWindow::on_spawnIntervalChanged(int value) {
    if (spawning) {
        spawnTimer->start(value * 1000);
    }
}

void MainWindow::on_btnPauseResumeCars_clicked()
{
    carsPaused = !carsPaused;

    if (carsPaused) {
        ui->btnPauseResumeCars->setText("Continue simulation");
        spawnTimer->stop();

        if (simulationRunning) {
            simulationElapsedMs += elapsedTimer.elapsed();
            simulationRunning = false;
        }

        for (CarSpawner* spawner : carSpawners)
            spawner->stop();

        if (groupA) groupA->pause();
        if (groupB) groupB->pause();

    } else {
        ui->btnPauseResumeCars->setText("Stop simulation");

        if (spawning) {
            int intervalMs = ui->spinSpawnInterval->value() * 1000;
            spawnTimer->start(intervalMs);
        }

        if (!simulationRunning) {
            simulationRunning = true;
            elapsedTimer.restart();
        }

        if (groupA) groupA->resume();
        if (groupB) groupB->resume();
    }

    for (Car* car : activeCars)
        carsPaused ? car->pause() : car->resume();
}

void MainWindow::addActiveCar(Car* car) {
    activeCars.append(car);
    if (activeCars.size() > maxCarsActive)
        maxCarsActive = activeCars.size();
    updateCarDataTable();
}

void MainWindow::removeActiveCar(Car* car) {
    activeCars.removeOne(car);

    if (selectedCar == car) {
        selectedCar = nullptr;

        if (selectedCarPathItem) {
            scene->removeItem(selectedCarPathItem);
            delete selectedCarPathItem;
            selectedCarPathItem = nullptr;
        }
    }

    updateCarDataTable();
}

void MainWindow::on_btnRestartSimulation_clicked() {
    if (simulationRunning) {
        QMessageBox::warning(this, "Warning", "The simulation needs to be stopped first!");
        return;
    }

    spawning = false;
    carsPaused = false;
    if (spawnTimer) spawnTimer->stop();
    if (simulationTimer) simulationTimer->stop();

    for (CarSpawner* spawner : carSpawners)
        spawner->stop();

    if (selectedCarPathItem) {
        scene->removeItem(selectedCarPathItem);
        delete selectedCarPathItem;
        selectedCarPathItem = nullptr;
    }
    selectedCar = nullptr;

    for (CarSpawner* spawner : carSpawners) {
        QVector<Car*>& cars = spawner->getCars();
        for (Car* car : cars) {
            car->stop();
            disconnect(car, nullptr, nullptr, nullptr);
            car->setVisible(false);
            car->deleteLater();
        }
        cars.clear();
    }

    activeCars.clear();

    totalCarsFinished = 0;
    totalCarsSpawned = 0;
    simulationElapsedMs = 0;
    maxCarsActive = 0;
    allTravelTimes.clear();
    allDistances.clear();
    percentStoppedHistory.clear();
    metricTimestamps.clear();

    ui->lblSimulationTime->setText("Simulation time: 00:00:000");
    ui->carDataTable->clearContents();
    ui->carDataTable->setRowCount(0);
    ui->btnSpawnDespawn->setText("Start vehicle spawning");
    ui->btnPauseResumeCars->setText("Stop simulation");

    for (auto tl : graph->trafficLights.values()) {
        tl->resume();
    }

    elapsedTimer.restart();
    simulationTimer->start(50);
    simulationRunning = true;

    if (spawning) {
        int intervalMs = ui->spinSpawnInterval->value() * 1000;
        for (CarSpawner* spawner : carSpawners) {
            spawner->startSpawning(intervalMs);
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (!ui) return;
    if (!ui->graphicsView) return;
    if (!scene) return;

    //ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio); Esticar mapa
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

void MainWindow::setupCarDataTableStyle() {
    ui->carDataTable->setColumnCount(2);
    ui->carDataTable->setHorizontalHeaderLabels(QStringList() << "Metrics" << "Value");

    ui->carDataTable->setStyleSheet(R"(
        QTableWidget {
            background-color: #1e1e1e;
            color: #ffffff;
            gridline-color: #444444;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #2c2c2c;
            color: #ffffff;
            padding: 6px;
            font-weight: bold;
            border: 1px solid #444444;
        }
        QTableWidget::item {
            padding: 4px;
        }
    )");


    ui->carDataTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->carDataTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->carDataTable->verticalHeader()->setDefaultSectionSize(50);
    ui->carDataTable->setAlternatingRowColors(false);
    ui->carDataTable->setShowGrid(true);
    ui->carDataTable->verticalHeader()->setVisible(false);
    ui->carDataTable->horizontalHeader()->setHighlightSections(false);
    ui->carDataTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->carDataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

    qint64 totalMs = simulationElapsedMs;
    if (simulationRunning) {
        totalMs += elapsedTimer.elapsed();
    }
    double currentSimulationTime = totalMs / 1000.0;
    metricTimestamps.append(currentSimulationTime);

    double meanPercentStopped = 0.0;
    if (!percentStoppedHistory.isEmpty()) {
        double sum = 0.0;
        for (double v : percentStoppedHistory) sum += v;
        meanPercentStopped = sum / percentStoppedHistory.size();
    }
    double trafficFlow = 100.0 - meanPercentStopped;

    ui->carDataTable->setRowCount(9);
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

    ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Average velocity per car (km/h):"));
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

    if (selectedCar) {
        double totalLength = 0.0;
        QVector<QPointF> path = selectedCar->getPath();
        for (int i = 1; i < path.size(); ++i) {
            totalLength += QLineF(path[i - 1], path[i]).length();
        }

        double progress = totalLength > 0.0
                              ? 100.0 * selectedCar->getTotalDistance() / totalLength
                              : 0.0;

        ui->carDataTable->insertRow(row);
        QTableWidgetItem* sectionHeader = new QTableWidgetItem(" Selected car info ");
        sectionHeader->setFlags(Qt::ItemIsEnabled);
        sectionHeader->setBackground(Qt::lightGray);
        sectionHeader->setTextAlignment(Qt::AlignCenter);
        ui->carDataTable->setItem(row, 0, sectionHeader);
        ui->carDataTable->setSpan(row, 0, 1, 2);
        row++;

        ui->carDataTable->insertRow(row);
        ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Speed (km/h):"));
        ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(selectedCar->getCurrentSpeed(), 'f', 2)));

        ui->carDataTable->insertRow(row);
        ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Distance (m):"));
        ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(selectedCar->getTotalDistance(), 'f', 2)));


        ui->carDataTable->insertRow(row);
        ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Travel time (s):"));
        ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(selectedCar->getElapsedTravelTimeMs() / 1000.0, 'f', 2)));

        ui->carDataTable->insertRow(row);
        ui->carDataTable->setItem(row++, 0, new QTableWidgetItem("Progress (%):"));
        ui->carDataTable->setItem(row - 1, 1, new QTableWidgetItem(QString::number(progress, 'f', 1)));
    } else {
        ui->carDataTable->insertRow(row);
        QTableWidgetItem* noCarItem = new QTableWidgetItem("Click on a car to show more info");
        noCarItem->setFlags(Qt::ItemIsEnabled);
        noCarItem->setTextAlignment(Qt::AlignCenter);
        noCarItem->setBackground(Qt::lightGray);
        ui->carDataTable->setItem(row, 0, noCarItem);
        ui->carDataTable->setSpan(row, 0, 1, 2);
    }
}

void MainWindow::on_btnShowCharts_clicked() {
    qDebug() << "Show charts clicked";
    if (!simulationRunning) {
        QMessageBox::warning(this, "Warning", "Simulation not running!");
        return;
    }
    showChartsDialog();
}

void MainWindow::showChartsDialog() {
    if (chartsDialog && chartsDialog->isVisible()) {
        chartsDialog->raise();
        chartsDialog->activateWindow();
        return;
    }

    struct MetricData {
        QString title;
        QVector<double> values;
        QString yLabel;
        QColor color;
    };

    struct ChartsData {
        QVector<MetricData> metrics;
        int currentMetric = 0;
    };

    auto* chartsData = new ChartsData;

    chartsData->metrics = {
        {"Percentage of stopped cars (%)", percentStoppedHistory, "Stopped (%)", QColor("#00B8A9")},
        {"Average traffic flow (%)", QVector<double>(), "Flow (%)", QColor("#F9A826")}
    };

    QVector<double> trafficFlow;
    for (double v : percentStoppedHistory) trafficFlow.append(100.0 - v);
    chartsData->metrics[1].values = trafficFlow;

    chartsDialog = new QDialog(this);
    chartsDialog->setAttribute(Qt::WA_DeleteOnClose);
    chartsDialog->setWindowTitle("Metrics evolution");
    QVBoxLayout* vbox = new QVBoxLayout(chartsDialog);

    QChartView* chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    vbox->addWidget(chartView);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    QPushButton* btnNext = new QPushButton("Next metric");
    btnLayout->addWidget(btnNext);
    vbox->addLayout(btnLayout);

    auto updateChart = [=]() {
        const MetricData& m = chartsData->metrics[chartsData->currentMetric];
        QLineSeries* series = new QLineSeries();
        series->setColor(m.color);

        for (int i = 0; i < m.values.size(); ++i) {
            if (i < metricTimestamps.size())
                series->append(metricTimestamps[i], m.values[i]);
        }

        QChart* chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(m.title);
        chart->legend()->hide();
        chart->setBackgroundBrush(QBrush(QColor("#F6F8F9")));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor("#F6F8F9")));
        chart->setPlotAreaBackgroundVisible(true);

        QValueAxis* axisX = new QValueAxis;
        axisX->setTitleText("Simulation time (s)");
        axisX->setLabelFormat("%.1f");
        axisX->setTickCount(10);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis* axisY = new QValueAxis;
        axisY->setTitleText(m.yLabel);
        axisY->setLabelFormat("%.1f");
        axisY->setRange(0, 100);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chartView->setChart(chart);
    };

    connect(btnNext, &QPushButton::clicked, chartsDialog, [=]() {
        chartsData->currentMetric = (chartsData->currentMetric + 1) % chartsData->metrics.size();
        updateChart();
    });

    updateChart();
    chartsDialog->resize(650, 400);
    chartsDialog->show();

    QObject::connect(chartsDialog, &QDialog::destroyed, [=]() {
        delete chartsData;
    });
}

void MainWindow::displayCarInfo(Car* car) {
    if (!car) return;

    if (selectedCarPathItem) {
        scene->removeItem(selectedCarPathItem);
        delete selectedCarPathItem;
        selectedCarPathItem = nullptr;
    }

    selectedCar = car;

    QVector<QPointF> path = car->getPath();
    if (path.size() >= 2) {
        QPainterPath painterPath(path[0]);
        for (int i = 1; i < path.size(); ++i) {
            painterPath.lineTo(path[i]);
        }

        selectedCarPathItem = scene->addPath(
            painterPath,
            QPen(QColor(255, 0, 0, 180), 2, Qt::DashLine)
            );
        selectedCarPathItem->setZValue(-0.5);
    }
}
