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
    ui->pushButton_6->setHidden(true);
    ui->pushButton_7->setHidden(true);

}

float initialPopulationSizeF;
float initialPopulationSizeUF;
float proportionRatio;
float mutationRate;
float h2OGroundChange;
float nursePlants;
int generation;
int rngSeed;
float temperatureChange;

bool gridIsMade;
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
void MainWindow::CreateGrid()
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
    //appends.push_back(1, 2);
    if(!gridIsMade)
    {
        CreateGrid();
    }
    ui->pushButton_2->setHidden(false);
    ui->pushButton->setHidden(true);
    ui->pushButton_7->setHidden(false);
}
//Close Graph
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton_7->setHidden(true);
    ui->pushButton_2->setHidden(true);
    ui->pushButton->setHidden(false);
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
    ui->tableWidget->setHidden(false);
    ui->pushButton_2->setHidden(false);
}
//close grid
void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setHidden(false);
    ui->pushButton_7->setHidden(true);
    ui->tableWidget->setHidden(true);
    ui->pushButton_2->setHidden(true);
}
