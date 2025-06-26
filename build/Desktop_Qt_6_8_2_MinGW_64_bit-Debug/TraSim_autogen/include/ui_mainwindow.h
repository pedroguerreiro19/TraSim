/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionIniciar;
    QAction *actionParar;
    QAction *actionReiniciar;
    QAction *actionMostrar_dados;
    QAction *actionSobre;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lblSimulationTime;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout_2;
    QLabel *lblSimulationInfo;
    QTableWidget *carDataTable;
    QPushButton *btnShowCharts;
    QLabel *lblSimulationChanges;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnSpawnDespawn;
    QLabel *labelSpawnInterval;
    QSpinBox *spinSpawnInterval;
    QPushButton *btnPauseResumeCars;
    QPushButton *btnRestartSimulation;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1904, 807);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon/app_icon.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionIniciar = new QAction(MainWindow);
        actionIniciar->setObjectName("actionIniciar");
        actionParar = new QAction(MainWindow);
        actionParar->setObjectName("actionParar");
        actionReiniciar = new QAction(MainWindow);
        actionReiniciar->setObjectName("actionReiniciar");
        actionMostrar_dados = new QAction(MainWindow);
        actionMostrar_dados->setObjectName("actionMostrar_dados");
        actionSobre = new QAction(MainWindow);
        actionSobre->setObjectName("actionSobre");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        lblSimulationTime = new QLabel(centralwidget);
        lblSimulationTime->setObjectName("lblSimulationTime");
        lblSimulationTime->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(lblSimulationTime);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(5);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setMinimumSize(QSize(1500, 720));

        verticalLayout->addWidget(graphicsView);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        lblSimulationInfo = new QLabel(centralwidget);
        lblSimulationInfo->setObjectName("lblSimulationInfo");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lblSimulationInfo->sizePolicy().hasHeightForWidth());
        lblSimulationInfo->setSizePolicy(sizePolicy2);
        lblSimulationInfo->setMaximumSize(QSize(16777215, 100));
        lblSimulationInfo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(lblSimulationInfo);

        carDataTable = new QTableWidget(centralwidget);
        carDataTable->setObjectName("carDataTable");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(carDataTable->sizePolicy().hasHeightForWidth());
        carDataTable->setSizePolicy(sizePolicy3);
        carDataTable->setMaximumSize(QSize(16777215, 15900000));

        verticalLayout_2->addWidget(carDataTable);

        btnShowCharts = new QPushButton(centralwidget);
        btnShowCharts->setObjectName("btnShowCharts");

        verticalLayout_2->addWidget(btnShowCharts);

        lblSimulationChanges = new QLabel(centralwidget);
        lblSimulationChanges->setObjectName("lblSimulationChanges");
        sizePolicy2.setHeightForWidth(lblSimulationChanges->sizePolicy().hasHeightForWidth());
        lblSimulationChanges->setSizePolicy(sizePolicy2);
        lblSimulationChanges->setMaximumSize(QSize(16777215, 500));
        lblSimulationChanges->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(lblSimulationChanges);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");

        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        btnSpawnDespawn = new QPushButton(centralwidget);
        btnSpawnDespawn->setObjectName("btnSpawnDespawn");
        btnSpawnDespawn->setEnabled(true);

        horizontalLayout_4->addWidget(btnSpawnDespawn);

        labelSpawnInterval = new QLabel(centralwidget);
        labelSpawnInterval->setObjectName("labelSpawnInterval");
        sizePolicy.setHeightForWidth(labelSpawnInterval->sizePolicy().hasHeightForWidth());
        labelSpawnInterval->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(labelSpawnInterval);

        spinSpawnInterval = new QSpinBox(centralwidget);
        spinSpawnInterval->setObjectName("spinSpawnInterval");
        spinSpawnInterval->setMinimum(2);
        spinSpawnInterval->setMaximum(5);
        spinSpawnInterval->setSingleStep(1);
        spinSpawnInterval->setValue(4);

        horizontalLayout_4->addWidget(spinSpawnInterval);


        verticalLayout_2->addLayout(horizontalLayout_4);

        btnPauseResumeCars = new QPushButton(centralwidget);
        btnPauseResumeCars->setObjectName("btnPauseResumeCars");

        verticalLayout_2->addWidget(btnPauseResumeCars);

        btnRestartSimulation = new QPushButton(centralwidget);
        btnRestartSimulation->setObjectName("btnRestartSimulation");

        verticalLayout_2->addWidget(btnRestartSimulation);


        horizontalLayout->addLayout(verticalLayout_2);


        horizontalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1904, 21));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TraSim - Simulador de tr\303\242nsito", nullptr));
        actionIniciar->setText(QCoreApplication::translate("MainWindow", "Iniciar", nullptr));
        actionParar->setText(QCoreApplication::translate("MainWindow", "Parar", nullptr));
        actionReiniciar->setText(QCoreApplication::translate("MainWindow", "Reiniciar ", nullptr));
        actionMostrar_dados->setText(QCoreApplication::translate("MainWindow", "Mostrar dados", nullptr));
        actionSobre->setText(QCoreApplication::translate("MainWindow", "Sobre", nullptr));
        lblSimulationTime->setText(QCoreApplication::translate("MainWindow", "Simulation time: 00:00", nullptr));
        lblSimulationInfo->setText(QCoreApplication::translate("MainWindow", "Simulation Info", nullptr));
        btnShowCharts->setText(QCoreApplication::translate("MainWindow", "Show Charts", nullptr));
        lblSimulationChanges->setText(QCoreApplication::translate("MainWindow", "Simulation settings", nullptr));
        btnSpawnDespawn->setText(QCoreApplication::translate("MainWindow", "Start vehicle spawning", nullptr));
        labelSpawnInterval->setText(QCoreApplication::translate("MainWindow", "Time between spawns (s):", nullptr));
        btnPauseResumeCars->setText(QCoreApplication::translate("MainWindow", "Stop simulation", nullptr));
        btnRestartSimulation->setText(QCoreApplication::translate("MainWindow", "Restart simulation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
