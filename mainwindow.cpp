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
    ui(new Ui::MainWindow),
    mChart(new QChart),
    mScene(new QGraphicsScene),
    mSeriesFacilitated(new QSplineSeries),
    mSeriesUnfacilitated(new QSplineSeries)
{
    ui->setupUi(this);

    //Setup all chart-related things
    mChartView = new QChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);
    ui->widget->layout()->addWidget(mChartView);
    mChartView->setScene(mScene);
    mScene->addItem(mChart);

    //m_image = new QImage();
    //ui->pushButton_2->setHidden(true);
    ui->pushButton_6->setHidden(true);
    //ui->pushButton_7->setHidden(true);

    //RJCB: Load from resources file, spatial-model.qrc
    m_image.load(":/pics/Grid.png");

    ui->label_grid->setPixmap(QPixmap::fromImage(m_image));
    ui->label_10->setHidden(true);

    //Make all parameters involved in initialization trigger CreateGrid
    QObject::connect(ui->spinBox_init_n_f, SIGNAL(valueChanged(int)), this, SLOT(ShowStart()));
    QObject::connect(ui->spinBox_init_n_uf, SIGNAL(valueChanged(int)), this, SLOT(ShowStart()));
    QObject::connect(ui->spinBox_n_nurse, SIGNAL(valueChanged(int)), this, SLOT(ShowStart()));
    QObject::connect(ui->spinBox_rng_seed, SIGNAL(valueChanged(int)), this, SLOT(ShowStart()));
    ShowStart();
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
    ui->label_grid->setMinimumHeight(400);
    ui->label_grid->setMaximumHeight(400);
    ui->label_10->setHidden(true);

    CreateGrid();
}

#ifdef TOO_SIMPLE_TO_KEEP_IN
Freely assume all STL functions are know by all C++ programmers
void MainWindow::set_seed()
{
    //RJCB: simplified
    std::srand(ui->spinBox_rng_seed->value());
}
#endif

//sets proportion ratio F and UF

void MainWindow::SetPixel(const int x, const int y, const QColor color)
{
  //RJCB: This '35 +' and '11 +' is completely weird,
  //  it is needed to draw the grid at the right spot somehow
  m_image.setPixel(35 + x, 11 + y,color.rgb());
  //m_image.setPixel(x, y,color.rgb());
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
#ifdef DELETE_UNUSED_CODE
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
#endif // DELETE_UNUSED_CODE

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
    while (static_cast<int>(nurse_plant_coordinats.size())
      < ui->spinBox_n_nurse->value())
    {
        //Pick a random spot to potentially put a nurse plant
        const int x=rand() % g[0].size(); //RJCB: Made more local and const
        const int y=rand() % g.size(); //RJCB: Made more local and const
        const coordinat c(x, y); //RJCB: Made more local and const
        if (is_good_spot_for_nurse_plant(c, nurse_plant_coordinats))
        {
          nurse_plant_coordinats.push_back(c);
          assert(y >= 0);
          assert(y < static_cast<int>(g.size()));
          assert(x >= 0);
          assert(x < static_cast<int>(g[y].size()));
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
    int n_facilitated_plants_to_add = ui->spinBox_init_n_f->value();
    int n_unfacilitated_plants_to_add = ui->spinBox_init_n_uf->value();
    assert(n_facilitated_plants_to_add >= 0);
    assert(n_unfacilitated_plants_to_add >= 0);
    plant_coordinats seeds;

    //Create as much facilitated and unfacilitated plants as needed
    while(n_facilitated_plants_to_add+n_unfacilitated_plants_to_add>0)
    {
      //Create a random seed
      const int x=rand() % g[0].size();
      const int y=rand() % g.size();
      const coordinat c(x,y);
      seeds.push_back(c);
      //Determine what this seed will add up to:
      //the number of facilitated of unfacilitated plants?
      position_in_relation_to_plants(
        g,
        nurse_plant,
        seeds,
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
    const int n_rows = g.size();
    for(int i=0; i<n_rows; ++i)
    {
        assert(i >= 0);
        assert(i < static_cast<int>(g.size()));
        const int n_cols = g[i].size();
        for(int j=0; j<n_cols; ++j)
        {
            assert(j >= 0);
            assert(j < static_cast<int>(g[i].size()));
            SetPixel(i,j,g[i][j]);
        }
    }
}

void MainWindow::set_plants(yx_grid& g)
{
    plant_coordinats nurse_plant_coordinats;
    plant_coordinats facilitated_plant;
    plant_coordinats unfacilitated_plant;
    add_nurse_plants(g, nurse_plant_coordinats);
    assert(static_cast<int>(nurse_plant_coordinats.size()) == ui->spinBox_n_nurse->value());
    set_facilitated_and_unfacilitated_plants(g, nurse_plant_coordinats, facilitated_plant, unfacilitated_plant);
    if(ui->spinBox_generation->value() > 0)
    {
        GenerateGeneration(g, nurse_plant_coordinats);
    }
}
void MainWindow::CreateGrid()
{
    //usingColor = blue;
    //SetGridResolution();

    //RJCB: set seed here
    std::srand(ui->spinBox_rng_seed->value());
    auto grid = create_vector_grid(60,50,brown);
    set_plants(grid);
    DrawGrid(grid);
    this->update();
}
void MainWindow::RemoveGrid()
{
    CreateGrid();
}

//creates graph
void MainWindow::ShowFitnessGraph()
{
  //Create a timeseries from 0.0-10.0 in steps of 0.1
  const int n_points{100};
  std::vector<double> xs;
  for (int i=0; i!=n_points; ++i)
  {
    xs.push_back(static_cast<double>(i) / 10.0);
  }
  //Unfacilitated plant fitnesses
  std::vector<double> ufs;
  for (int i=0; i!=n_points; ++i)
  {
    const double x{xs[i]};
    const double u_optimum = 3.0;
    const double u_sd = 1.0;
    ufs.push_back(normal(x, u_optimum, u_sd));
  }

  std::vector<double> ffs;
  for (int i=0; i!=n_points; ++i)
  {
    const double x{xs[i]};
    const double f_optimum = 6.0;
    const double f_sd = 2.0;
    ffs.push_back(normal(x, f_optimum, f_sd));
  }

  mChart->show();

  mSeriesFacilitated->clear();
  //appends F plants
  for (int i=0; i!=n_points; ++i)
  {
      mSeriesFacilitated->append(xs[i], ffs[i]);
  }
  mChart->addSeries(mSeriesFacilitated);
  mSeriesUnfacilitated->clear();
  //appends UF plants
  for (int i=0; i!=n_points; ++i)
  {
      mSeriesUnfacilitated->append(xs[i], ufs[i]);
  }
  mChart->addSeries(mSeriesUnfacilitated);

   //mChart->legend()->hide();
   mChart->createDefaultAxes();
   mChart->setTitle("How fitness depends on your neighbours");
   mChart->show();
   QValueAxis *axisX = new QValueAxis;
   axisX->setTitleText("Trait value");
   QValueAxis *axisY = new QValueAxis;
   axisY->setTitleText("Fitness");
   mChart->setAxisX(axisX, mSeriesFacilitated);
   mChart->setAxisY(axisY, mSeriesFacilitated);

}
void MainWindow::GenerateGeneration(yx_grid& g, plant_coordinats &nurse_plant)
{
     nurse_plants_seeds(nurse_plant, g);
    //produce new plants
    //remove dead plants
}
void MainWindow::setPopulationSize(double value)
{
    if(ui->spinBox_init_n_f->value() != 0)
    {
        value = value * ui->spinBox_init_n_f->value();
    }
    ui->spinBox_init_n_uf->setValue(value);
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

#ifdef DO_THIS_WHEN_NEEDED
Just read those values at the start of a simulation
void MainWindow::on_spinBox_n_f_valueChanged(int arg1)
{
    initialPopulationSizeF = arg1;
}
//value chnaged UF spinbox
void MainWindow::on_spinBox_n_uf_valueChanged(int arg1)
{
    initialPopulationSizeUF = arg1;
}
#endif // DO_THIS_WHEN_NEEDED

//proportion ratio slider
void MainWindow::on_proportionRatioUF_valueChanged(double arg1)
{
  setPopulationSize(arg1);
}
//generation slider
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->spinBox_generation->setValue(value);
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
/*
void MainWindow::on_spinBox_n_nurse_valueChanged(int arg1)
{
    mNnursePlants = arg1;
}
*/

//H2O ground change
void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    h2OGroundChange = arg1;
}
//RNG
void MainWindow::on_spinBox_4_valueChanged(int)
{
    //mRngSeed = arg1;
    mGeneration = 0;
    ui->horizontalSlider_2->setValue(0);
    ui->spinBox_generation->setValue(0);

    //RJCB: set seed directly
    std::srand(ui->spinBox_rng_seed->value());
}
//show grid
void MainWindow::on_pushButton_6_clicked()
{
    ui->pushButton_7->setHidden(false);
    ui->pushButton_6->setHidden(true);
    //ui->tableWidget->setHidden(false);
    ui->label_grid->setMinimumHeight(400);
    ui->label_grid->setMaximumHeight(400);
    CreateGrid();

}
//close grid
void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setHidden(false);
    ui->pushButton_7->setHidden(true);
    //ui->tableWidget->setHidden(true);
    ui->label_grid->setMaximumHeight(0);
    ui->label_grid->setMinimumHeight(0);
    RemoveGrid();
}

//run
void MainWindow::on_pushButton_run_pressed()
{
    ui->label_grid->setMaximumHeight(0);
    ui->label_grid->setMinimumHeight(0);
    ui->label_10->setHidden(false);
    RemoveGrid();
    generation_coordinates.clear();

}
//run
void MainWindow::on_pushButton_run_released()
{
    ui->label_grid->setMinimumHeight(400);
    ui->label_grid->setMaximumHeight(400);
    ui->label_10->setHidden(true);
    CreateGrid();
}
//reset
void MainWindow::on_pushButton_clicked()
{
    //proportionRatio=0;
    mutationRate=0;
    h2OGroundChange=0;
    mGeneration=0;
    //mRngSeed=0;
    temperatureChange=0;
    ui->spinBox_init_n_f->setValue(0);
    ui->spinBoxMutation->setValue(0);
    ui->spinBox_init_n_uf->setValue(0);
    ui->spinBox_generation->setValue(0);
    ui->spinBox_n_nurse->setValue(0);
    ui->doubleSpinBox_7->setValue(0);
    delay();
}

///-----------------------------------------------------------------------------
///RJCB put his functions here sorted alphabetically,
/// as there appeared to be no order in the place of functions
///-----------------------------------------------------------------------------

void MainWindow::ShowStart()
{
    CreateGrid();
    ShowFitnessGraph();
}

/*
void MainWindow::ClearGrid(const yx_grid& g)
{
    const int n_rows = g.size();
    for(int i=0; i<n_rows; ++i)
    {
        assert(i >= 0);
        assert(i < static_cast<int>(g.size()));
        const int n_cols = g[i].size();
        for(int j=0; j<n_cols; ++j)
        {
            assert(j >= 0);
            assert(j < static_cast<int>(g[i].size()));
            SetPixel(i,j,brown);
        }
    }
}
*/

yx_grid create_vector_grid(
  const int n_rows,
  const int n_columns,
  QColor color
)
{
  ///RJCB: simplified it a bit
  assert(n_rows > 0);
  assert(n_columns > 0);
  return yx_grid(n_rows, std::vector<QColor>(n_columns, color));
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

double gauss(const double x, const double sd) noexcept
{
  return std::exp(-(x * x) / (2.0 * sd * sd));
}

double normal(const double x, const double mean, const double sd) noexcept
{
  return gauss(x - mean, sd);
}
