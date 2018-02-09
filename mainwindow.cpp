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

    //RJCB: Load from resources file, spatial-model.qrc
    m_image.load(":/pics/Grid.png");

    ui->label_5->setPixmap(QPixmap::fromImage(m_image));
    ui->label_10->setHidden(true);

    QObject::connect(ui->spinBox_n_f, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));

    CreateGrid();
    CreateGraph();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::delay()
{
    QTime waitTime = QTime::currentTime().addMSecs(1);

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
    qCritical() << "XStart: " << gridXStart;
    qCritical() << "XEnd: " << gridXEnd;
    qCritical() << "YStart: " << gridYStart;
    qCritical() << "YEnd: " << gridYEnd;
}*/
//draws grid on GUI
//unused
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

#ifdef RJCB_REFACTORED
Moved to add_nurse_plants

/// nurse_plant_coordinats.back() has a candidate seed
void MainWindow::distance_between_nurse_plants(
  plant_coordinats &nurse_plant_coordinats,
  int &add_nurse_plant_i,
  yx_grid& g
)
{
    bool distance_enough=true;
    /* Moved to is_good_spot_for_nurse_plant

    int n_nurse_plant_coordinats=nurse_plant_coordinats.size();
    for(int i=0; i<n_nurse_plant_coordinats; ++i)
    {
        if(i!=n_nurse_plant_coordinats-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            const int position_difference_x=std::abs(nurse_plant_coordinats[i].first - nurse_plant_coordinats[n_nurse_plant_coordinats-1].first);
            const int position_difference_y=std::abs(nurse_plant_coordinats[i].second - nurse_plant_coordinats[n_nurse_plant_coordinats-1].second);
            if(position_difference_x<=1 && position_difference_y<=1)
            {
                distance_enough=false;
            }
        }
    }
    */
    if(distance_enough==true)
    {
         g[nurse_plant_coordinats[n_nurse_plant_coordinats-1].second][nurse_plant_coordinats[n_nurse_plant_coordinats-1].first]=blue;
    }
    else
    {
        add_nurse_plant_i+=1;
        nurse_plant_coordinats.pop_back();
    }
}
#endif // RJCB_REFACTORED


///RJCB: renamed to 'add_nurse_plants' from from 'set_nurse_plant'
///  as nurse plants are added
///Adds mNnursePlants to both grid and nurse_plant_coordinats
void MainWindow::add_nurse_plants(
  yx_grid& g,
  plant_coordinats &nurse_plant_coordinats
)
{
    const int n_nurse_plants = mNnursePlants;

    for(int i=0; i<n_nurse_plants; ++i)
    {
        //Pick a random spot to potentially put a nurse plant
        const int x=rand() % g[0].size(); //RJCB: Made more local and const
        const int y=rand() % g.size(); //RJCB: Made more local and const
        const coordinat c(x, y); //RJCB: Made more local and const
        if (is_good_spot_for_nurse_plant(c, nurse_plant_coordinats))
        {
          nurse_plant_coordinats.push_back(c);
          g[c.second][c.first]=blue;

        }
        //RJCB: Use this approach instead of                            C
        // - adding a seed to nurse_plant_coordinats                    O
        // - see if it works                                            M
        //   - if yes, keep the seed, add it to grid                    P
        //   - if no, remove the seed from nurse_plant_coordinats       L
        //                                                              E
        //                                                              X
        //
        //                                                              !
        //
        //distance_between_nurse_plants(nurse_plant_coordinats, n_nurse_plants, g);
    }
}

void MainWindow::position_check_facilitated_plant(
  int dx, //RJCB: delta x, a standard name for the difference in horizontal positions
  int dy,
  bool &make_facilitated_plant,
  bool &make_unfacilitated_plant,
  bool &stop_unfaciliated,
  bool &stop_faciliated
)
{
    if(dx>1 && dy>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(dx>1 && dy==0 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(dx==0 && dy>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }

    if(dx==1 && dy>1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }
    if(dx>1 && dy==1 && stop_unfaciliated==false)
    {
       make_unfacilitated_plant=true;
    }


    if(dx ==1 && dy==1 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }
    if(dx ==0 && dy==1 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }
    if(dx ==1 && dy==0 && stop_faciliated==false)
    {
        make_facilitated_plant=true;
        make_unfacilitated_plant=false;
        stop_unfaciliated=true;
    }

    if(dx==0 && dy==0)
    {
       make_facilitated_plant=false;
       make_unfacilitated_plant=false;
       stop_faciliated=true;
       stop_unfaciliated=true;
    }
}
bool MainWindow::distance_between_facilitated_plants(const plant_coordinats facilitated_plant)
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

            position_difference_x=std::abs(position_difference_x);
            position_difference_y=std::abs(position_difference_y);

            if(position_difference_x==0 && position_difference_y==0)
            {
                return false;
            }
        }
    }
    return true;
}
bool MainWindow::distance_between_unfacilitated_plants(const plant_coordinats unfacilitated_plant)
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

            position_difference_x=std::abs(position_difference_x);
            position_difference_y=std::abs(position_difference_y);

            if(position_difference_x==0 && position_difference_y==0)
            {
                return false;
            }
        }
    }
    return true;
}

///RJCB: Please as a friend what he/she expects from a function
///  with name 'position_in_relation_to_plants'.
///  No idea what this function did at first!
/// 'put_seed'?
/// @param g grid that will have a seed added
void MainWindow::position_in_relation_to_plants(
  yx_grid& g,
  const plant_coordinats& nurse_plant_coordinats, //RJCB: Those were not nurse plants, those were nurse plant coordinats
  const plant_coordinats& seed_coordinats, //RJCB: Those were not seed, those were seed coordinats
  plant_coordinats &facilitated_plant,
  plant_coordinats &unfacilitated_plant,
  const coordinat& c,
  int &n_facilitated_plants_to_add,
  int &n_unfacilitated_plants_to_add
)
{
    bool make_facilitated_plant=false;
    bool make_unfacilitated_plant=false;
    bool stop_unfacilitated=false;
    bool stop_facilitated=false;

    //RJCB: just iterator over all coordinats
    //npc: Nurse Plant Coordinat
    //See if the seed has fallen close to a nurse plant
    for(const auto& npc: nurse_plant_coordinats)
    {
        const auto& last_seed_coordinat = seed_coordinats.back();
        const int dx=std::abs(npc.first - last_seed_coordinat.first);
        const int dy=std::abs(npc.second - last_seed_coordinat.second);
        position_check_facilitated_plant(
            dx,
            dy,
            make_facilitated_plant,
            make_unfacilitated_plant,
            stop_unfacilitated,
            stop_facilitated
        );
    }
    if(make_facilitated_plant==true && n_facilitated_plants_to_add>0)
    {
         g[seed_coordinats[seed_coordinats.size()-1].second][seed_coordinats[seed_coordinats.size()-1].first]=green;
         facilitated_plant.push_back(c);
         if(distance_between_facilitated_plants(facilitated_plant)==true)
         {
            --n_facilitated_plants_to_add;
         }
         else
         {
             facilitated_plant.pop_back();
         }
    }
    if(make_unfacilitated_plant==true && n_unfacilitated_plants_to_add>0)
    {
         g[seed_coordinats[seed_coordinats.size()-1].second][seed_coordinats[seed_coordinats.size()-1].first]=white;
         unfacilitated_plant.push_back(c);
         if(distance_between_unfacilitated_plants(unfacilitated_plant)==true)
         {
            --n_unfacilitated_plants_to_add;
         }
         else
         {
            unfacilitated_plant.pop_back();
         }
    }
}
void MainWindow::set_facilitated_and_unfacilitated_plants(
  yx_grid& g,
  plant_coordinats &nurse_plant,
  plant_coordinats &facilitated_plant,
  plant_coordinats &unfacilitated_plant
)
{
    int n_facilitated_plants_to_add = initialPopulationSizeF;
    int n_unfacilitated_plants_to_add = initialPopulationSizeUF;
    assert(n_facilitated_plants_to_add >= 0);
    assert(n_unfacilitated_plants_to_add >= 0);
    plant_coordinats seeds;

    //Create as much facilitated and unfacilitated plants as needed
    while(n_facilitated_plants_to_add+n_unfacilitated_plants_to_add>0)
    {
      //Create a random seed
      qCritical() << ".";
      const int x=rand() % g[0].size();
      const int y=rand() % g.size();
      const coordinat c(x,y);
      seeds.push_back(c);
      //int position_difference_x = 0; //RJCB: Unused here
      //int position_difference_y = 0; //RJCB: Unused here
      //Determine what this seed will add up to:
      //the number of facilitated of unfacilitated plants?
      position_in_relation_to_plants(
        g,
        nurse_plant,
        seeds,
        //position_difference_x, //RJCB: Unused here
        //position_difference_y, //RJCB: Unused here
        facilitated_plant,
        unfacilitated_plant,
        c,
        n_facilitated_plants_to_add,
        n_unfacilitated_plants_to_add
      );
    }

}
void MainWindow::save_generation(plant_coordinats &nurse_plant)
{
    for(int i=0; i<mGeneration; ++i)
    {
        generation_coordinates.push_back(nurse_plant);
    }
}
void MainWindow::set_seed_nurse_plant_coordinates(int x, int y, int &nurse_plant_x, int &nurse_plant_y, plant_coordinats nurse_plant, int i)
{
    nurse_plant_y = nurse_plant[i].second;
    nurse_plant_y+=y;
    nurse_plant_x = nurse_plant[i].first;
    nurse_plant_x+=x;

}
void MainWindow::check_seed_nurse_plant_coordinates(
  int x,
  int y,
  int &nurse_plant_x,
  int &nurse_plant_y,
  plant_coordinats &nurse_plant,
  int i,
  plant_coordinats /* seed_coordinate */, //RJCB: Unused argument, so I just comment it out
  yx_grid& g
 )
{
    std::cout<< "nurse plant first: "<<nurse_plant[i].first<<"\n";
    while(nurse_plant_x < 0 || nurse_plant_x > (static_cast<int>(g[0].size()) - 1)) //RJCB: must static_cast std::size_t to int
    {
        x = (rand() % 7) - 3;
        nurse_plant_x = nurse_plant[i].first;
        nurse_plant_x+=x;
    }
    while(nurse_plant_y < 0 || nurse_plant_y > (static_cast<int>(g.size()) - 1)) //RJCB: must static_cast std::size_t to int
    {
        y = (rand() % 7) - 3;
        nurse_plant_y = nurse_plant[i].second;
        nurse_plant_y+=y;
    }
/*
    assert(nurse_plant_y>0);
    assert(nurse_plant_y<g.size());

    assert(nurse_plant_x>0);
    assert(nurse_plant_x<g[0].size());
    */
}
void MainWindow::new_generation(plant_coordinats seed_coordinate, plant_coordinats &nurse_plant, yx_grid& g)
{
    for(unsigned i=0; i<seed_coordinate.size(); ++i)
    {
        nurse_plant.push_back(seed_coordinate[i]);
        g[nurse_plant[i].second][nurse_plant[i].first]=red;
    }
}

void MainWindow::nurse_plants_seeds(plant_coordinats &nurse_plant, yx_grid& g)
{
    save_generation(nurse_plant);
    for(int i=0; i<mGeneration; ++i)
    {
    plant_coordinats seed_coordinate;
    coordinat c=coordinat(0,0);
    for(unsigned i=0; i<nurse_plant.size(); ++i)
    {
        int x = (rand() % 7) - 3;
        int y = (rand() % 7) - 3;
        int nurse_plant_x=0;
        int nurse_plant_y=0;
        set_seed_nurse_plant_coordinates(x,y,nurse_plant_x,nurse_plant_y,nurse_plant, i);
        check_seed_nurse_plant_coordinates(x,y, nurse_plant_x, nurse_plant_y, nurse_plant, i, seed_coordinate, g);
        c=coordinat(nurse_plant_x,nurse_plant_y);
        seed_coordinate.push_back(c);
    }
    nurse_plant.clear();
    new_generation(seed_coordinate, nurse_plant, g);

    }
}



void MainWindow::DrawGrid(const yx_grid& g)
{
    qCritical() << __FUNCTION__;
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
    qCritical() << __FUNCTION__;
    plant_coordinats nurse_plant;
    qCritical() << __LINE__;
    plant_coordinats facilitated_plant;
    qCritical() << __LINE__;
    plant_coordinats unfacilitated_plant;
    qCritical() << __LINE__;
    add_nurse_plants(g, nurse_plant);
    qCritical() << __LINE__;
    set_facilitated_and_unfacilitated_plants(g, nurse_plant, facilitated_plant, unfacilitated_plant);
    qCritical() << __LINE__;
    #ifdef NOT_NOW
    if(m_generation > 0)
    {
        GenerateGeneration(g, nurse_plant);
    }
    #endif // NOT_NOW
}
void MainWindow::CreateGrid()
{
    qCritical() << __FUNCTION__;
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
void MainWindow::GenerateGeneration(yx_grid& g, plant_coordinats &nurse_plant)
{
     nurse_plants_seeds(nurse_plant, g);
    //produce new plants
    //remove dead plants
}
void MainWindow::setPopulationSize(double value)
{
    if(ui->spinBox_n_f->value() != 0)
    {
        value = value * ui->spinBox_n_f->value();
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
void MainWindow::on_spinBox_n_f_valueChanged(int arg1)
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
    mGeneration = arg1;

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
    mNnursePlants = arg1;
}
//H2O ground change
void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    h2OGroundChange = arg1;
}
//RNG
void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    mRngSeed = arg1;
    mGeneration = 0;
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
void MainWindow::on_pushButton_run_pressed()
{
    ui->label_5->setMaximumHeight(0);
    ui->label_5->setMinimumHeight(0);
    ui->label_10->setHidden(false);
    RemoveGrid();
    generation_coordinates.clear();

}
//run
void MainWindow::on_pushButton_run_released()
{
    ui->label_5->setMinimumHeight(400);
    ui->label_5->setMaximumHeight(400);
    ui->label_10->setHidden(true);
    CreateGrid();
}
//reset
void MainWindow::on_pushButton_clicked()
{
    mNnursePlants = 0;
    initialPopulationSizeF = 0;
    initialPopulationSizeUF = 0;
    proportionRatio=0;
    mutationRate=0;
    h2OGroundChange=0;
    mNnursePlants=0;
    mGeneration=0;
    mRngSeed=0;
    temperatureChange=0;
    ui->spinBox_n_f->setValue(0);
    ui->spinBoxMutation->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);
    ui->doubleSpinBox_5->setValue(0);
    ui->doubleSpinBox_7->setValue(0);
    delay();
}

bool is_good_spot_for_nurse_plant(
  const coordinat& c,
  const plant_coordinats& nurse_plant_coordinats,
  const int min_dx,
  const int min_dy
)
{
    // npc: Nurse Plant Coordinat
    for(const auto& npc: nurse_plant_coordinats)
    {
        const int dx=std::abs(npc.first - c.first);
        const int dy=std::abs(npc.second - c.second);
        // If too close to a nurse plant, that is not a good spot
        if(dx<=min_dx && dy<=min_dy) // smaller-equal, as it is the number of squares *between* coordinats
        {
            return false;
        }
    }
    return true;
}
