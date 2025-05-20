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

    QVector<CarSpawner*> carSpawners;
private slots:
    void spawnCarRandomly();
    void on_actionIniciar_triggered();
    void on_actionParar_triggered();
    void on_actionReiniciar_triggered();
    void on_actionSair_triggered();
    void mousePressEvent(QMouseEvent *event) override;
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Ui::MainWindow *ui;
    CarSpawner* carSpawner;
    Graph *graph;
    QTimer *spawnTimer;

    void setupScene();
protected:
    void resizeEvent(QResizeEvent* evt) override;
    void showEvent(QShowEvent* event) override;
};
#endif // MAINWINDOW_H
