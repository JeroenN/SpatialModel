#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "explanation.h"
#include <QApplication>
#include <vector>
#include <iostream>
#include <QtCharts>
#include <string>
#include <QtGui>
using namespace QtCharts;
#include <QtQuick/QtQuick>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setHidden(true);
    ui->pushButton_6->setHidden(true);
    ui->pushButton_7->setHidden(true);
    CreateGrid();
    CreateGraph();

}

MainWindow::~MainWindow()
{
    delete ui;
}
//sets proportion ratio F and UF
void MainWindow::setProportionratio()
{
    if(initialPopulationSizeUF == 0)
    {
        ui->proportionRatioUF->setValue(0);
    }
    if(initialPopulationSizeF != 0)
    {
        proportionRatio = initialPopulationSizeUF / initialPopulationSizeF;
        ui->proportionRatioUF->setValue(proportionRatio);
    }
}
void MainWindow::CreateGrid()
{
    /*
    QQuickView *gridView = new QQuickView;
    QWidget *gridContainer = QWidget::createWindowContainer(gridView, this);
    gridContainer->setMinimumSize(200,200);
    gridContainer->setMaximumSize(200,200);
    gridContainer->setFocusPolicy(Qt::TabFocus);
    gridView->setSource(QUrl("Grid.qml"));
    ui->gridLayout->addWidget(gridContainer);*/
    /*
    QQmlEngine engine;
    QQmlComponent component(&engine,
            QUrl::fromLocalFile("/home/florian/C++/Projects/SpatialModel/Grid.qml"));
    QObject *object = component.create();
    qDebug() << "Property value:" << QQmlProperty::read(object, "gridSize").toInt();
    QQmlProperty::write(object, "gridSize", 100);

    qDebug() << "Property value:" << object->property("gridSize").toInt();
    object->setProperty("gridSize", 100);
    */
    QQmlEngine engine;
    QQmlComponent component(&engine,
            QUrl::fromLocalFile("/home/florian/C++/Projects/SpatialModel/Grid.qml"));
    QObject *object = component.create();

    gridWidget = new QQuickWidget(this);
    gridWidget->setSource(QUrl("/home/florian/C++/Projects/SpatialModel/Grid.qml"));

    ui->gridLayout->addWidget(gridWidget, 1, 0);




    QObject *rect = object->findChild<QObject*>("gridBox");
    if (rect)
        rect->setProperty("color", "blue");





    for(int i = 0; i < 250; i++)
    {
        /*
        qDebug() << "Property value:" << QQmlProperty::read(object, "gridSize").toInt();
        QQmlProperty::write(object, "gridSize", 10);

        qDebug() << "Property value:" << object->property("gridSize").toInt();
        object->setProperty("gridSize", 10);
        */
        QObject *rect = object->findChild<QObject*>("grid");
        if (rect)
            rect->setProperty("color", "blue");

    }

    /*
    gridScene = new QGraphicsScene;
    QGraphicsView *gridView = new QGraphicsView;

    gridView->setScene(gridScene);
    for(int x = 0; x<=500; x+=50)
        scene->addLine((x,0,x,500), QPen(Qt::red));
    for(int y = 0; y<=500; y+=50)
        scene->addLine((0,y,500,y), QPen(Qt::green));
    gridView.fitInView(scene->itemsVBoundingRect());
*/
}
//creates graph
void MainWindow::CreateGraph()
{
    //test code
    fXAppends.push_back(1);
    fYAppends.push_back(1);
    fXAppends.push_back(1.5);
    fYAppends.push_back(3);
    fXAppends.push_back(2);
    fYAppends.push_back(10);
    fXAppends.push_back(2.5);
    fYAppends.push_back(3);
    fXAppends.push_back(3);
    fYAppends.push_back(1);


    uFXAppends.push_back(1);
    uFYAppends.push_back(0.5);
    uFXAppends.push_back(1.5);
    uFYAppends.push_back(3.5);
    uFXAppends.push_back(2);
    uFYAppends.push_back(10.5);
    uFXAppends.push_back(2.5);
    uFYAppends.push_back(3);
    uFXAppends.push_back(3);
    uFYAppends.push_back(0.5);
    //end test code
    chart = new QChart();
    scene = new QGraphicsScene;
    fSeries = new QSplineSeries();
    uFSeries = new QSplineSeries();
    chart->show();

    //appends F plants
    for(int i = 0; i < fXAppends.size(); i++)
    {
        fSeries->append(fXAppends[i], fYAppends[i]);
    }
    chart->addSeries(fSeries);
    //appends UF plants
    for(int i = 0; i < uFXAppends.size(); i++)
    {
        uFSeries->append(uFXAppends[i], uFYAppends[i]);
    }
    chart->addSeries(uFSeries);


     chart->legend()->hide();
     chart->createDefaultAxes();
     chart->setTitle("Fitness");
     chart->show();
     chartView = new QChartView(chart);
     QValueAxis *axisX = new QValueAxis;
     axisX->setTitleText("Optimum");
     QValueAxis *axisY = new QValueAxis;
     axisY->setTitleText("Amount of F/UF plants");
     chart->setAxisX(axisX, fSeries);
     chart->setAxisY(axisY, fSeries);
     chartView->setRenderHint(QPainter::Antialiasing);
     ui->widget->layout()->addWidget(chartView);

     chartView->setScene(scene);
     scene->addItem(chart);

     gridIsMade = true;


}
void MainWindow::setPopulationSize(double value)
{
    if(ui->spinBox->value() != 0)
    {
        value = value * ui->spinBox->value();
    }
    ui->spinBox_2->setValue(value);
}
//show graph
void MainWindow::on_pushButton_clicked()
{
    if(!gridIsMade)
    {
        CreateGraph();
    }
    ui->pushButton_2->setHidden(false);
    ui->pushButton->setHidden(true);
    ui->pushButton_7->setHidden(false);
}
void MainWindow::DestroyGraph()
{
    //chart->removeSeries(fSeries);
    //chart->removeSeries(uFSeries);
    //chart->hide();
    //chartView->hide();
    //chartView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //chartView->setMaximumHeight(0);
    //scene->removeItem(chart);
    //ui->widget->layout()->removeWidget(chartView);
    //ui->widget->layout()->setEnabled(false);
    //chartView->setScene(scene);

   // chart->deleteLater();
    //scene->deleteLater();
    chartView->deleteLater();
    //delete chart;
    //delete scene;
    //delete chartView;

}
//Close Graph
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_7->setHidden(true);
    ui->pushButton_2->setHidden(true);
    ui->pushButton->setHidden(false);
    DestroyGraph();
}
//muation rate slider
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBoxMutation->setValue(value);
}
//mutation rate
void MainWindow::on_spinBoxMutation_valueChanged(double arg1)
{
    ui->horizontalSlider->setValue(arg1);
    mutationRate = arg1;
}
//value changed F spinbox
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    initialPopulationSizeF = arg1;
    setProportionratio();
}
//value chnaged UF spinbox
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    initialPopulationSizeUF = arg1;
    setProportionratio();
}
//proportion ratio slider
void MainWindow::on_proportionRatioUF_valueChanged(double arg1)
{
  setPopulationSize(arg1);
}
//generation slider
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->spinBox_3->setValue(value);
}
//generation
void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
    generation = arg1;
}
//show explanation
void MainWindow::on_pushButton_5_clicked()
{
    explanation = new Explanation();
    explanation->show();
}
//temperature change
void MainWindow::on_doubleSpinBox_7_valueChanged(double arg1)
{
    temperatureChange = arg1;
}
//fixed proportion of Nurse Plants
void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    nursePlants = arg1;
}
//H2O ground change
void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    h2OGroundChange = arg1;
}
//RNG seed
void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    rngSeed = arg1;
}
//show grid
void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_7->setHidden(false);
    ui->pushButton_6->setHidden(true);
    //ui->tableWidget->setHidden(false);
    ui->pushButton_2->setHidden(false);
}
//close grid
void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setHidden(false);
    ui->pushButton_7->setHidden(true);
    //ui->tableWidget->setHidden(true);
    ui->pushButton_2->setHidden(true);
}
