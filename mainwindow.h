#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"
#include "carspawner.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QHash>
#include <QPointF>

class Road;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void incrementCarsSpawned();
    void registerCarFinished(qint64 travelTimeMs, double distance);
    void saveStatisticsCSV();
    void showStatistics();
    void addActiveCar(Car* car);
    void removeActiveCar(Car* car);
    static MainWindow* instance();

    QVector<Car*> activeCars;
    QVector<CarSpawner*> carSpawners;
private slots:
    void spawnCarRandomly();
    void on_btnStartSim_clicked();
    void on_btnStopSim_clicked();
    void on_btnDespawnCars_clicked();
    void mousePressEvent(QMouseEvent *event) override;
private:
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    CarSpawner* carSpawner;
    Graph *graph;
    QTimer *spawnTimer;
    static MainWindow* m_instance;
    void updateCarDataTable();

    int totalCarsSpawned = 0;
    int totalCarsFinished = 0;
    QVector<qint64> allTravelTimes;
    QVector<double> allDistances;

    void setupScene();
protected:
    void resizeEvent(QResizeEvent* evt) override;
    void showEvent(QShowEvent* event) override;
};
#endif // MAINWINDOW_H
