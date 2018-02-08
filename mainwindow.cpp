#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "explanation.h"
#include <QApplication>
#include <vector>
#include <iostream>
#include <QtCharts>
#include <string>
#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <cassert>
using namespace QtCharts;
#include <QtQuick/QtQuick>


//grid
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //m_image = new QImage();
    //ui->pushButton_2->setHidden(true);
    ui->pushButton_6->setHidden(true);
    //ui->pushButton_7->setHidden(true);
    m_image.load("/home/florian/C++/Projects/SpatialModel/Grid.png");
    ui->label_5->setPixmap(QPixmap::fromImage(m_image));
    ui->label_10->setHidden(true);
    CreateGrid();
    CreateGraph();


}

MainWindow::~MainWindow()
{
    delete ui;
}
int positive_maker(int number)
{
    if(number<0)
    {
        number*=-1;
    }
    return number;
}
void MainWindow::delay()
{
    QTime waitTime = QTime::currentTime().addMSecs(1);
    qDebug() << "Wait";

    while(QTime::currentTime() < waitTime)
     {
        QCoreApplication::processEvents(QEventLoop::AllEvents , 100);
     }
    ui->label_5->setMinimumHeight(400);
    ui->label_5->setMaximumHeight(400);
    ui->label_10->setHidden(true);

    CreateGrid();
}
void MainWindow::set_seed()
{
    int seed;
    seed = ui->spinBox_4->value();
    srand (seed);
}
//sets proportion ratio F and UF

void MainWindow::SetPixel(const int x, const int y, const QColor color)
{
  m_image.setPixel(35 + x, 11 + y,color.rgb());
}

void MainWindow::SetResolution(const int width, const int height)
{
  m_image = QImage(width,height,QImage::Format_RGB32);
}

void MainWindow::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    QPixmap::fromImage(m_image)
  );
}
/*void MainWindow::SetGridResolution()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    gridXStart = screenGeometry.width() / 40;
    gridYStart = screenGeometry.height() / 75;
    gridXEnd = screenGeometry.width() / 14;
    gridYEnd = screenGeometry.height() / 12;
    qDebug() << "XStart: " << gridXStart;
    qDebug() << "XEnd: " << gridXEnd;
    qDebug() << "YStart: " << gridYStart;
    qDebug() << "YEnd: " << gridYEnd;
}*/
//draws grid on GUI
void MainWindow::DrawGrid2()
{
    for(int x = gridXStart; x < gridXEnd; x++)
    {
        for(int y = gridYStart; y < gridYEnd; y++)
        {
            SetPixel(x,y, usingColor);
        }
    }
}

void MainWindow::distance_between_nurse_plants(plant_coordinates &nurse_plant, int &add_nurse_plant_i, yx_grid& g)
{
    int position_difference_x;
    int position_difference_y;
    int n_nurse_plant=nurse_plant.size();
    bool distance_enough=true;

    for(int i=0; i<n_nurse_plant; ++i)
    {
        if(i!=n_nurse_plant-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            position_difference_x= nurse_plant[i].first - nurse_plant[n_nurse_plant-1].first;
            position_difference_y= nurse_plant[i].second - nurse_plant[n_nurse_plant-1].second;

            position_difference_x=positive_maker(position_difference_x);
            position_difference_y=positive_maker(position_difference_y);

            if(position_difference_x<=1 && position_difference_y<=1)
            {
                distance_enough=false;
            }
        }
    }
    if(distance_enough==true)
    {
         g[nurse_plant[n_nurse_plant-1].second][nurse_plant[n_nurse_plant-1].first]=blue;
    }
    else
    {
        add_nurse_plant_i+=1;
        nurse_plant.pop_back();
    }
}
void MainWindow::set_nurse_plant(yx_grid& g, plant_coordinates &nurse_plant)
{
    int n_nurse_plants= nursePlants;
    //assert n_nur
    int x=0;
    int y=0;
    coordinate c=coordinate(0,0);

    for(int i=0; i<n_nurse_plants; ++i)
    {
        x=rand() % g[0].size();
        y=rand() % g.size();
        c = coordinate(x, y);
        nurse_plant.push_back(c);
        distance_between_nurse_plants(nurse_plant, n_nurse_plants, g);
    }
}

void MainWindow::position_check_facilitated_plant(int position_difference_x, int position_difference_y, bool &make_facilitated_plant,
                                      bool &make_unfacilitated_plant, bool &stop_unfaciliated, bool &stop_faciliated)
{
    if(position_difference_x>1 && position_difference_y>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(position_difference_x>1 && position_difference_y==0 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(position_difference_x==0 && position_difference_y>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }

    if(position_difference_x==1 && position_difference_y>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(position_difference_x>1 && position_difference_y==1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }


    if(position_difference_x ==1 && position_difference_y==1 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }
    if(position_difference_x ==0 && position_difference_y==1 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }
    if(position_difference_x ==1 && position_difference_y==0 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }

    if(position_difference_x==0 && position_difference_y==0)
    {
       make_facilitated_plant=false;
       make_unfacilitated_plant=false;
       stop_faciliated=true;
       stop_unfaciliated=true;
    }
}
bool MainWindow::distance_between_facilitated_plants(const plant_coordinates facilitated_plant)
{
    int position_difference_x;
    int position_difference_y;
    int n_facilitated_plant=facilitated_plant.size();

    for(int i=0; i<n_facilitated_plant; ++i)
    {
        if(i!=n_facilitated_plant-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            position_difference_x= facilitated_plant[i].first - facilitated_plant[n_facilitated_plant-1].first;
            position_difference_y= facilitated_plant[i].second - facilitated_plant[n_facilitated_plant-1].second;

            position_difference_x=positive_maker(position_difference_x);
            position_difference_y=positive_maker(position_difference_y);

            if(position_difference_x==0 && position_difference_y==0)
            {
                return false;
            }
        }
    }
    return true;
}
bool MainWindow::distance_between_unfacilitated_plants(const plant_coordinates unfacilitated_plant)
{
    int position_difference_x;
    int position_difference_y;
    int n_unfacilitated_plant=unfacilitated_plant.size();

    for(int i=0; i<n_unfacilitated_plant; ++i)
    {
        if(i!=n_unfacilitated_plant-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            position_difference_x= unfacilitated_plant[i].first - unfacilitated_plant[n_unfacilitated_plant-1].first;
            position_difference_y= unfacilitated_plant[i].second - unfacilitated_plant[n_unfacilitated_plant-1].second;

            position_difference_x=positive_maker(position_difference_x);
            position_difference_y=positive_maker(position_difference_y);

            if(position_difference_x==0 && position_difference_y==0)
            {
                return false;
            }
        }
    }
    return true;
}
void MainWindow::position_in_relation_to_plants(yx_grid& g, plant_coordinates nurse_plant, plant_coordinates seeds, int &position_difference_x, int &position_difference_y,
                                    plant_coordinates &facilitated_plant, plant_coordinates &unfacilitated_plant, coordinate c, int &n_facilitated_plant
                                    , int &n_unfacilitated_plant)
{
    int n_plant=seeds.size();
    bool make_facilitated_plant=false;
    bool make_unfacilitated_plant=false;
    bool stop_unfacilitated=false;
    bool stop_facilitated=false;
    for(unsigned i=0; i<nurse_plant.size(); ++i)
    {
            position_difference_x= nurse_plant[i].first - seeds[n_plant-1].first;
            position_difference_y= nurse_plant[i].second - seeds[n_plant-1].second;
            position_difference_x=positive_maker(position_difference_x);
            position_difference_y=positive_maker(position_difference_y);
            position_check_facilitated_plant(position_difference_x, position_difference_y, make_facilitated_plant, make_unfacilitated_plant, stop_unfacilitated, stop_facilitated);
    }
    if(make_facilitated_plant==true && n_facilitated_plant>0)
    {
         g[seeds[seeds.size()-1].second][seeds[seeds.size()-1].first]=green;
         facilitated_plant.push_back(c);
         if(distance_between_facilitated_plants(facilitated_plant)==true)
         {
            n_facilitated_plant-=1;
         }
         else
         {
             facilitated_plant.pop_back();
         }
    }
    if(make_unfacilitated_plant==true && n_unfacilitated_plant>0)
    {
         g[seeds[seeds.size()-1].second][seeds[seeds.size()-1].first]=white;
         unfacilitated_plant.push_back(c);
         if(distance_between_unfacilitated_plants(unfacilitated_plant)==true)
         {
            n_unfacilitated_plant-=1;
         }
         else
         {
             unfacilitated_plant.pop_back();
         }
    }
}
void MainWindow::set_facilitated_and_unfacilitated_plants(yx_grid& g, plant_coordinates nurse_plant, plant_coordinates &facilitated_plant, plant_coordinates &unfacilitated_plant)
{
    int position_difference_x;
    int position_difference_y;
    int x=0;
    int y=0;
    int n_facilitated_plants= initialPopulationSizeF;
    int n_unfacilitated_plants= initialPopulationSizeUF;
    plant_coordinates seeds;
    coordinate c=coordinate(0,0);

    //while(plants.size()!=n_facilitated_plants+n_unfacilitated_plants)
    while(n_facilitated_plants+n_unfacilitated_plants>0)
    {
        x=rand() % g[0].size();
        y=rand() % g.size();
        c=coordinate(x,y);
        seeds.push_back(c);
        position_in_relation_to_plants(g,nurse_plant, seeds, position_difference_x, position_difference_y, facilitated_plant, unfacilitated_plant, c,
                                       n_facilitated_plants, n_unfacilitated_plants);
    }

}
void MainWindow::nurse_plants_seeds(plant_coordinates &nurse_plant, yx_grid& g)
{
    for(int i=0; i<generation; ++i)
    {
    generation_coordinates.push_back(nurse_plant);
    plant_coordinates seed_coordinate;
    coordinate c=coordinate(0,0);
    for(unsigned i=0; i<nurse_plant.size(); ++i)
    {
        int x = (rand() % 7) - 3;
        int y = (rand() % 7) - 3;
        int nurse_plant_x=0;
        int nurse_plant_y=0;

        nurse_plant_y = nurse_plant[i].second;
        nurse_plant_y+=y;
        while(nurse_plant_y < 0 || nurse_plant_y > g.size())
        {
            y = (rand() % 7) - 3;
            nurse_plant_y = nurse_plant[i].second;
            nurse_plant_y+=y;
        }
        nurse_plant_x = nurse_plant[i].first;
        nurse_plant_x+=x;
        while(nurse_plant_x< 0 || nurse_plant_x > g[0].size())
        {
            x = (rand() % 7) - 3;
            nurse_plant_x = nurse_plant[i].first;
            nurse_plant_x+=x;
        }
        c=coordinate(nurse_plant_x,nurse_plant_y);
        seed_coordinate.push_back(c);
    }
    nurse_plant.clear();
    for(unsigned i=0; i<seed_coordinate.size(); ++i)
    {
        nurse_plant.push_back(seed_coordinate[i]);
        g[nurse_plant[i].second][nurse_plant[i].first]=red;
    }
    }
}


void MainWindow::DrawGrid(const yx_grid& g)
{
    const int n_rows = g.size();
    for(int i=0; i<n_rows; ++i)
    {
        assert(i >= 0);
        assert(i < static_cast<int>(g.size()));
        const int n_cols = g[i].size();
        for(int j=0; j<n_cols; ++j)
        {
            assert(j < static_cast<int>(g[i].size()));
            SetPixel(i,j,g[i][j]);
        }
    }
}
void MainWindow::set_plants(yx_grid& g)
{
    plant_coordinates nurse_plant;
    plant_coordinates facilitated_plant;
    plant_coordinates unfacilitated_plant;
    set_nurse_plant(g, nurse_plant);
    set_facilitated_and_unfacilitated_plants(g, nurse_plant, facilitated_plant, unfacilitated_plant);
    if(generation >0)
    {
        GenerateGeneration(g, nurse_plant);
    }

}
void MainWindow::CreateGrid()
{
    usingColor = blue;
    //SetGridResolution();
    set_seed();
    auto grid = create_vector_grid(60,50);
    set_plants(grid);
    DrawGrid(grid);
}
void MainWindow::RemoveGrid()
{
    usingColor = white;
    CreateGrid();
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
    for(unsigned i = 0; i < fXAppends.size(); i++)
    {
        fSeries->append(fXAppends[i], fYAppends[i]);
    }
    chart->addSeries(fSeries);
    //appends UF plants
    for(unsigned i = 0; i < uFXAppends.size(); i++)
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

}
void MainWindow::GenerateGeneration(yx_grid& g, plant_coordinates nurse_plant)
{

     nurse_plants_seeds(nurse_plant, g);
    //produce new plants
    //remove dead plants
}
void MainWindow::setPopulationSize(double value)
{
    if(ui->spinBox->value() != 0)
    {
        value = value * ui->spinBox->value();
    }
    ui->spinBox_2->setValue(value);
}
//button slots
//-------------------
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
}
//value chnaged UF spinbox
void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    initialPopulationSizeUF = arg1;
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
void MainWindow::on_spinBox_5_valueChanged(int arg1)
{
    nursePlants = arg1;
}
//H2O ground change
void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    h2OGroundChange = arg1;
}
//RNG
void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    rngSeed = arg1;
    generation = 0;
    ui->horizontalSlider_2->setValue(0);
    ui->spinBox_3->setValue(0);
    set_seed();
}
//show grid
void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_7->setHidden(false);
    ui->pushButton_6->setHidden(true);
    //ui->tableWidget->setHidden(false);
    ui->label_5->setMinimumHeight(400);
    ui->label_5->setMaximumHeight(400);
    CreateGrid();

}
//close grid
void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setHidden(false);
    ui->pushButton_7->setHidden(true);
    //ui->tableWidget->setHidden(true);
    ui->label_5->setMaximumHeight(0);
    ui->label_5->setMinimumHeight(0);
    RemoveGrid();
}
//run
void MainWindow::on_pushButton_3_pressed()
{
    ui->label_5->setMaximumHeight(0);
    ui->label_5->setMinimumHeight(0);
    ui->label_10->setHidden(false);
    RemoveGrid();
    generation_coordinates.clear();

}
//run
void MainWindow::on_pushButton_3_released()
{
    ui->label_5->setMinimumHeight(400);
    ui->label_5->setMaximumHeight(400);
    ui->label_10->setHidden(true);
    CreateGrid();
}
//reset
void MainWindow::on_pushButton_clicked()
{
    nursePlants = 0;
    initialPopulationSizeF = 0;
    initialPopulationSizeUF = 0;
    proportionRatio=0;
    mutationRate=0;
    h2OGroundChange=0;
    nursePlants=0;
    generation=0;
    rngSeed=0;
    temperatureChange=0;
    ui->spinBox->setValue(0);
    ui->spinBoxMutation->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_7->setValue(0);
    delay();
}

//generation
void MainWindow::on_spinBox_3_editingFinished()
{
    //std::cout<<generationCheck;

    //if(generationCheck == false)
    //{
        //generationCheck = true;
        //delay();
    //}
}
