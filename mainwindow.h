#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QTimer>
#include <QPointer>
#include <QDialog>
#include <QList>
#include <QVector>
#include <QElapsedTimer>
#include <QHash>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "graph.h"
#include "carspawner.h"
#include "trafficlightgroup.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Acesso Singleton
    static MainWindow* instance();

    // Gestão de carros
    void addActiveCar(Car* car);
    void removeActiveCar(Car* car);
    void displayCarInfo(Car* car);
    void incrementCarsSpawned();
    void registerCarFinished(qint64 travelTimeMs, double distance);

private slots:
    // Slots de interação UI
    void on_btnSpawnDespawn_clicked();
    void on_btnPauseResumeCars_clicked();
    void on_btnRestartSimulation_clicked();
    void on_btnShowCharts_clicked();
    void on_spawnIntervalChanged(int value);
    void visualizarNodes(); // Eliminar depois
    void spawnCarRandomly();
    void showChartsDialog();

protected:
    // Eventos de UI
    void resizeEvent(QResizeEvent* evt) override;
    void showEvent(QShowEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    // UI e Cena
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Graph *graph;
    QGraphicsPathItem* selectedCarPathItem = nullptr;

    // Estado da simulação
    static MainWindow* m_instance;
    QTimer *spawnTimer = nullptr;
    QTimer *simulationTimer = nullptr;
    QElapsedTimer elapsedTimer;
    bool spawning = false;
    bool carsPaused = false;
    bool simulationRunning = false;
    int simulationSeconds = 0;
    qint64 simulationElapsedMs = 0;

    // Carros e spawners
    Car* selectedCar = nullptr;
    QVector<Car*> activeCars;
    QVector<CarSpawner*> carSpawners;

    // Métricas
    int totalCarsSpawned = 0;
    int totalCarsFinished = 0;
    int maxCarsActive = 0;
    QVector<qint64> allTravelTimes;
    QVector<double> allDistances;
    QVector<double> percentStoppedHistory;
    QVector<double> metricTimestamps;

    // Semáforos
    TrafficLightGroup* groupA = nullptr;
    TrafficLightGroup* groupB = nullptr;

    // Janelas auxiliares
    QPointer<QDialog> chartsDialog = nullptr;

    // Setup
    void setupScene();
    void updateCarDataTable();
};

#endif // MAINWINDOW_H
