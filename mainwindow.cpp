#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "explanation.h"

//#include <QtCharts>
using namespace QtCharts;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setHidden(true);

}
float initialPopulationSizeF = 0;
float initialPopulationSizeUF = 0;
float proportionRatio = 0;


MainWindow::~MainWindow()
{
    delete ui;
}

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
void MainWindow::setPopulationSize(double value)
{
    //ui->spinBox->setValue(1);
    if(ui->spinBox->value() != 0)
    {
        value = value * ui->spinBox->value();
    }
    ui->spinBox_2->setValue(value);
}

void MainWindow::on_pushButton_clicked()
{

    QLineSeries *series = new QLineSeries();

     series->append(0, 6);

     series->append(10, 5);
     QChart *chart = new QChart();

     QGraphicsScene * scene = new QGraphicsScene;

     chart->legend()->hide();
     chart->addSeries(series);
     chart->createDefaultAxes();
     chart->setTitle("Graph");
     chart->show();
     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);
     ui->widget->layout()->addWidget(chartView);
     chartView->setScene(scene);
     scene->addItem(chart);
    ui->pushButton_2->setHidden(false);
    ui->pushButton->setHidden(true);


}

void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBoxMutation->setValue(value);
}

void MainWindow::on_spinBoxMutation_valueChanged(double arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    initialPopulationSizeF = arg1;
    setProportionratio();
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    initialPopulationSizeUF = arg1;
    setProportionratio();

}

void MainWindow::on_proportionRatioUF_valueChanged(double arg1)
{
  setPopulationSize(arg1);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->spinBox_3->setValue(value);
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void MainWindow::on_pushButton_5_clicked()
{
    explanation = new Explanation();
    explanation->show();
}
