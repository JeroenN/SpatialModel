#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "explanation.h"
#include <QApplication>
#include <vector>
#include <iostream>
#include <random>
#include <QtCharts>
#include <string>
#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <cassert>
using namespace QtCharts;
#include <QtQuick/QtQuick>
#include "qtmutualismbreakdownerspatialwidget.h"

//grid
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mFitnessChart(new QChart),
    mFitnessSeriesFacilitated(new QLineSeries),
    mFitnessSeriesUnfacilitated(new QLineSeries),
    mNumberOfSeedsChart(new QChart),
    mCurrentTraitDistributionChart(new QChart),
    m_seagrass_widget{new ribi::mb::QtMutualismBreakdownerSpatialWidget(10,10)}
{
    ui->setupUi(this);

    //The scroll area's content must have a vertical layout
    {
      ui->results->setLayout(new QVBoxLayout);
    }
    //Grid
    {
      const auto my_layout = ui->results->layout();
      assert(my_layout);
      my_layout->addWidget(m_seagrass_widget);
      m_seagrass_widget->setMinimumSize(400,400);

    }
    //Setup all fitness chart-related things
    {

      mFitnessChartView = new QChartView(mFitnessChart);
      mFitnessChartView->setRenderHint(QPainter::Antialiasing);
      ui->results->layout()->addWidget(mFitnessChartView);
      mFitnessChart->setMinimumHeight(400);

      mFitnessChart->createDefaultAxes();
      mFitnessChart->setTitle("How fitness depends on your neighbours");
      mFitnessChart->addSeries(mFitnessSeriesUnfacilitated);
      mFitnessChart->addSeries(mFitnessSeriesFacilitated);
      QValueAxis *axisX = new QValueAxis;
      axisX->setRange(0,1.0);
      axisX->setTitleText("Trait value");
      QValueAxis *axisY = new QValueAxis;
      axisY->setTitleText("Fitness");
      //axisY->setRange(0,0.1);
      mFitnessChart->setAxisX(axisX);
      mFitnessChart->setAxisY(axisY);
      mFitnessSeriesFacilitated->setName("Facilitated");
      mFitnessSeriesUnfacilitated->setName("Unfacilitated");


      mActualValueLine = new QFrame(mFitnessChartView);
      mActualValueLine->setObjectName(QString::fromUtf8("line"));
      mActualValueLine->setFrameShape(QFrame::VLine);
      mActualValueLine->setFrameShadow(QFrame::Sunken);
    }
    //Charts
    {
      mNumberOfSeedsChartView = new QChartView(mNumberOfSeedsChart);
      mNumberOfSeedsChartView->setRenderHint(QPainter::Antialiasing);
      ui->results->layout()->addWidget(mNumberOfSeedsChartView);
      mNumberOfSeedsChart->setMinimumHeight(400);

      //mNumberOfSeedsChartView->setHidden(true);

      mNumberOfSeedsChart->createDefaultAxes();
      mNumberOfSeedsChart->setTitle("Number of seeds produced");
      mNumberOfSeedsFacilitatedSet = new QBarSet("Facilitated");
      mNumberOfSeedsUnfacilitatedSet = new QBarSet("Unfacilitated");
      *mNumberOfSeedsFacilitatedSet << 10;
      *mNumberOfSeedsUnfacilitatedSet << 100;
      QBarSeries *series = new QBarSeries();
      series->append(mNumberOfSeedsUnfacilitatedSet);
      series->append(mNumberOfSeedsFacilitatedSet);
      mNumberOfSeedsChart->addSeries(series);

      //mNumberOfSeedsChart->addSeries(mFitnessSeriesUnfacilitated);
      //mNumberOfSeedsChart->addSeries(mFitnessSeriesFacilitated);
      //QValueAxis *axisX = new QValueAxis;
      //axisX->setRange(0,1.0);
      //axisX->setTitleText("Trait value");
      //QValueAxis *axisY = new QValueAxis;
      //axisY->setTitleText("Fitness");
      //axisY->setRange(0,0.1);
      //mFitnessChart->setAxisX(axisX);
      //mFitnessChart->setAxisY(axisY);
      //mFitnessSeriesFacilitated->setName("Facilitated");
      //mFitnessSeriesUnfacilitated->setName("Unfacilitated");
    }
    //setup current trait distribution chart
    {
        mCurrentTraitDistributionView = new QChartView(mCurrentTraitDistributionChart);
        mCurrentTraitDistributionView->setRenderHint(QPainter::Antialiasing);
        ui->results->layout()->addWidget(mCurrentTraitDistributionView);
        mCurrentTraitDistributionChart->setMinimumHeight(400);
        //mCurrentTraitDistributionView->setHidden(true);

        mCurrentTraitDistributionChart->createDefaultAxes();
        mCurrentTraitDistributionChart->setTitle("Current trait distribution");
        QBarSeries *series = new QBarSeries();
        CurrentTraitdistributionSets = new QBarSet("Traits");
        *CurrentTraitdistributionSets << 16 << 30 <<22 << 56 << 22 << 21 << 30 << 12 << 12 << 78 << 70 << 30 <<22 << 56 << 22 << 21 << 30 << 12 << 12 << 78 << 21 << 0;
        QStringList categories;
        categories << ".00" << ".05" << ".10" << ".15" << ".20" << ".25" << ".30" << ".35" << ".40" << ".45" << ".50" << ".55" <<
                      ".60" << ".65" << ".70" << ".75" << ".80" << ".85" << ".90" << ".95" << "1.0";
        QBarCategoryAxis *axisX2 = new QBarCategoryAxis();
        axisX2->append(categories);
        axisX2->setRange(QString("0.00"), QString("1.00"));
        mCurrentTraitDistributionChart->setAxisX(axisX2);
        series->append(CurrentTraitdistributionSets);
        mCurrentTraitDistributionChart->addSeries(series);
    }

    //m_image = new QImage();
    //ui->pushButton_2->setHidden(true);
    ui->pushButton_6->setHidden(true);
    //ui->pushButton_7->setHidden(true);

    //Resolution grid grid
    connect(ui->box_grid_height, SIGNAL(valueChanged(int)), this, SLOT(SetResolution()));
    connect(ui->box_grid_width, SIGNAL(valueChanged(int)), this, SLOT(SetResolution()));
    SetResolution();

    //Make all parameters involved in initialization trigger CreateGrid
    QObject::connect(ui->spinBox_n_nurse, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));
    QObject::connect(ui->spinBox_rng_seed, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));
    QObject::connect(ui->spinBox_init_n_seeds, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));
    QObject::connect(ui->spinBox_traits_mean, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));
    QObject::connect(ui->spinBox_traits_dev, SIGNAL(valueChanged(int)), this, SLOT(CreateGrid()));
    CreateGrid();

    QObject::connect(ui->box_fit_fac_opt, SIGNAL(valueChanged(double)), this, SLOT(ShowGraphs()));
    QObject::connect(ui->box_fit_fac_sd, SIGNAL(valueChanged(double)), this, SLOT(ShowGraphs()));
    QObject::connect(ui->box_fit_unfac_opt, SIGNAL(valueChanged(double)), this, SLOT(ShowGraphs()));
    QObject::connect(ui->box_fit_unfac_sd, SIGNAL(valueChanged(double)), this, SLOT(ShowGraphs()));
    ShowGraphs();
}

MainWindow::~MainWindow()
{
    delete ui;
}
///GetRandomNormal draws a random number from a normal distribution
///with average mean and standard deviation of sigma.
double getRandomNormal(std::mt19937& mt,const double mean, const double sigma)
{
 /*
  double f= (double)rand() /RAND_MAX;
  //randomX is a value between sigma and -sigma
  double randomX = -sigma + f *(sigma*2); //fMin-fMax = sigma - - sigma = sigma + sigma = sigma*2
  double x = mean +randomX;
  */
   std::normal_distribution<double> d(mean, sigma);
   const double x{d(mt)};
  return x;
}

void MainWindow::delay()
{
    QTime waitTime = QTime::currentTime().addMSecs(1);

    while(QTime::currentTime() < waitTime)
     {
        QCoreApplication::processEvents(QEventLoop::AllEvents , 100);
     }
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
  this->m_seagrass_widget->SetPixel(x, y, color);
}

void MainWindow::SetResolution()
{
  m_seagrass_widget->SetResolution(
    ui->box_grid_width->value(),
    ui->box_grid_height->value()
  );
  CreateGrid();
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
bool MainWindow::distance_between_facilitated_plants(const plant_coordinats facilitated_plant_coordinates)
{
    int position_difference_x;
    int position_difference_y;
    int n_facilitated_plant=facilitated_plant_coordinates.size();

    for(int i=0; i<n_facilitated_plant; ++i)
    {
        if(i!=n_facilitated_plant-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            position_difference_x= facilitated_plant_coordinates[i].first - facilitated_plant_coordinates[n_facilitated_plant-1].first;
            position_difference_y= facilitated_plant_coordinates[i].second - facilitated_plant_coordinates[n_facilitated_plant-1].second;

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
bool MainWindow::distance_between_unfacilitated_plants(const plant_coordinats unfacilitated_plant_coordinates)
{
    int position_difference_x;
    int position_difference_y;
    n_unfacilitated_plant=unfacilitated_plant_coordinates.size();

    for(int i=0; i<n_unfacilitated_plant; ++i)
    {
        if(i!=n_unfacilitated_plant-1)  //This makes sure the nurse plant doesn't check the distance between its self
        {
            position_difference_x= unfacilitated_plant_coordinates[i].first - unfacilitated_plant_coordinates[n_unfacilitated_plant-1].first;
            position_difference_y= unfacilitated_plant_coordinates[i].second - unfacilitated_plant_coordinates[n_unfacilitated_plant-1].second;

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
  plant_coordinats &facilitated_plant_coordinates,
  plant_coordinats &unfacilitated_plant_coordinates,
  const coordinat& c,
  int &n_seeds_to_add,
  plant_values &plant_trait_values
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
    if(make_facilitated_plant==true && n_seeds_to_add>0)
    {
          QColor green= QColor(0, plant_trait_values[plant_trait_values.size()-1]*10+30, 0);
          facilitated_plant_trait_value.push_back(plant_trait_values[plant_trait_values.size()-1]);
         g[seed_coordinats[seed_coordinats.size()-1].second][seed_coordinats[seed_coordinats.size()-1].first]= green;
         facilitated_plant_coordinates.push_back(c);
         if(distance_between_facilitated_plants(facilitated_plant_coordinates)==true)
         {
            --n_seeds_to_add;
         }
         else
         {
             facilitated_plant_coordinates.pop_back();
             facilitated_plant_trait_value.pop_back();
         }
    }
    if(make_unfacilitated_plant==true && n_seeds_to_add>0)
    {
        QColor red= QColor(plant_trait_values[plant_trait_values.size()-1]*190+20,0, 0);
        unfacilitated_plant_trait_value.push_back(plant_trait_values[plant_trait_values.size()-1]);
         g[seed_coordinats[seed_coordinats.size()-1].second][seed_coordinats[seed_coordinats.size()-1].first]=red;
         unfacilitated_plant_coordinates.push_back(c);
         if(distance_between_unfacilitated_plants(unfacilitated_plant_coordinates)==true)
         {
            --n_seeds_to_add;
         }
         else
         {
            unfacilitated_plant_coordinates.pop_back();
            unfacilitated_plant_trait_value.pop_back();
         }
    }
}

void MainWindow::set_facilitated_and_unfacilitated_plants(
  yx_grid& g,
  plant_coordinats &nurse_plant,
  plant_coordinats &facilitated_plant_coordinates,
  plant_coordinats &unfacilitated_plant_coordinates,
  plant_values &plant_trait_values
)
{
    m_rng_engine = std::mt19937(ui->spinBox_rng_seed->value());
    int n_seeds_to_add = ui->spinBox_init_n_seeds->value();
    plant_coordinats seeds;

    //Create as much facilitated and unfacilitated plants as needed

    while(n_seeds_to_add>0)
    {
      //Create a random seed
      const int x=rand() % g[0].size();
      const int y=rand() % g.size();
      const coordinat c(x,y);
      seeds.push_back(c);
      plant_trait_values.push_back(getRandomNormal(m_rng_engine, ui->spinBox_traits_mean->value(),  ui->spinBox_traits_dev->value()));
      //Determine what this seed will add up to:
      //the number of facilitated of unfacilitated plants?
      set_plant_trait_next_gen(plant_trait_values);

      position_in_relation_to_plants(
        g,
        nurse_plant,
        seeds,
        facilitated_plant_coordinates,
        unfacilitated_plant_coordinates,
        c,
        n_seeds_to_add,
        plant_trait_values
      );
    }
    float total_traits_facilitated=0;
    for(unsigned i=0; i<facilitated_plant_trait_value.size(); ++i)
    {
        const double fitness_facilitated = normal(
          facilitated_plant_trait_value[i],//ui->box_trait_optimum->value(),
          ui->box_fit_fac_opt->value(),
          ui->box_fit_fac_sd->value()
        );
        facilitated_plant_fitness_value.push_back(fitness_facilitated);
    }
    total_traits_facilitated=facilitated_plant_trait_value.size();
    set_traits_next_gen(m_rng_engine, total_traits_facilitated);
    clear_fitness_trait_vectors();
    ShowCurrentTraitDistributionGraph();
    float average_trait_value_facilitated=total_traits_facilitated/facilitated_plant_trait_value.size();
    float percentage_facilitated=100*((float)facilitated_plant_coordinates.size()/((float)unfacilitated_plant_coordinates.size()+(float)facilitated_plant_coordinates.size()));//  /n_seeds
    float percentage_unfacilitated=100*((float)unfacilitated_plant_coordinates.size()/((float)unfacilitated_plant_coordinates.size()+(float)facilitated_plant_coordinates.size()));//  /n_seeds

    ShowNumberOfSeedsGraph(percentage_facilitated, percentage_unfacilitated, average_trait_value_facilitated);
}

void MainWindow::set_fitness_facilitated()
{
    for(unsigned i=0; i<facilitated_plant_trait_value.size(); ++i)
    {
        const double fitness_facilitated = normal(
          facilitated_plant_trait_value[i],//ui->box_trait_optimum->value(),
          ui->box_fit_fac_opt->value(),
          ui->box_fit_fac_sd->value()
        );
        facilitated_plant_fitness_value.push_back(fitness_facilitated);
    }
}

void MainWindow::set_fitness_unfacilitated()
{
    for(unsigned i=0; i<unfacilitated_plant_trait_value.size(); ++i)
    {
        const double fitness_unfacilitated = normal(
          unfacilitated_plant_trait_value[i],//ui->box_trait_optimum->value(),
          ui->box_fit_fac_opt->value(),
          ui->box_fit_fac_sd->value()
        );
        unfacilitated_plant_fitness_value.push_back(fitness_unfacilitated);
    }
}
void MainWindow::set_traits_next_gen(std::mt19937& mt, const int total_traits_facilitated)
{
       std::vector<double> fitness_all_plants;
       set_fitness_facilitated();
       set_fitness_unfacilitated();
       for(unsigned i=0; i!=facilitated_plant_fitness_value.size(); ++i)
       {
           fitness_all_plants.push_back(facilitated_plant_fitness_value[i]);
       }

       for(unsigned i=0; i!=unfacilitated_plant_fitness_value.size(); ++i)
       {
           fitness_all_plants.push_back(unfacilitated_plant_fitness_value[i]);
       }

       std::discrete_distribution<int> dist(fitness_all_plants.begin(), fitness_all_plants.end());
       std::cout<<"fitness 25: "<< fitness_all_plants[25] <<"\n";
       std::cout<<"facilitated size: "<< total_traits_facilitated <<"\n";
       for (int i=0; i!=ui->spinBox_init_n_seeds->value(); ++i)
       {
         std::cout << dist(mt) << ' ';
       }
      std::cout<<"\n";
}
void MainWindow::clear_fitness_trait_vectors()
{
    unfacilitated_plant_fitness_value.clear();
    facilitated_plant_fitness_value.clear();
    unfacilitated_plant_trait_value.clear();
    facilitated_plant_trait_value.clear();
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
        g[nurse_plant[i].second][nurse_plant[i].first]=white;
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
void MainWindow::set_plant_trait_next_gen( plant_values &plant_trait_values)
{
    plant_values next_generation_plant_traits;
    for(unsigned i=0; i<plant_trait_values.size(); ++i)
    next_generation_plant_traits.push_back(getRandomNormal(m_rng_engine, 0,  ui->spinBox_mutation_stddev->value()));
}

void MainWindow::set_plants(yx_grid& g)
{
    plant_coordinats nurse_plant_coordinats;
    plant_coordinats facilitated_plant_coordinates;
    plant_coordinats unfacilitated_plant_coordinates;
    plant_trait_values.clear();
    add_nurse_plants(g, nurse_plant_coordinats);
    assert(static_cast<int>(nurse_plant_coordinats.size()) == ui->spinBox_n_nurse->value());
    set_facilitated_and_unfacilitated_plants(g, nurse_plant_coordinats, facilitated_plant_coordinates, unfacilitated_plant_coordinates, plant_trait_values);
    if(ui->spinBox_generation->value() > 0)
    {
        GenerateGeneration(g, nurse_plant_coordinats);
    }
}
void MainWindow::CreateGrid()
{
    std::srand(ui->spinBox_rng_seed->value()); //set Seed
    auto grid = create_vector_grid(
      ui->box_grid_width->value(), // 65,
      ui->box_grid_height->value(), //60,
      brown
    );
    set_plants(grid);
    DrawGrid(grid);

    m_seagrass_widget->update();
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
    const double u_optimum = ui->box_fit_unfac_opt->value();
    const double u_sd = ui->box_fit_unfac_sd->value();
    ufs.push_back(normal(x, u_optimum, u_sd));
  }

  std::vector<double> ffs;
  for (int i=0; i!=n_points; ++i)
  {
    const double x{xs[i]};
    const double f_optimum = ui->box_fit_fac_opt->value();
    const double f_sd = ui->box_fit_fac_sd->value();
    ffs.push_back(normal(x, f_optimum, f_sd));
  }

  mFitnessChart->show();

  mFitnessSeriesFacilitated->clear();
  //appends F plants
  for (int i=0; i!=n_points; ++i)
  {
      mFitnessSeriesFacilitated->append(xs[i], ffs[i]);
  }
  mFitnessSeriesUnfacilitated->clear();
  //appends UF plants
  for (int i=0; i!=n_points; ++i)
  {
      mFitnessSeriesUnfacilitated->append(xs[i], ufs[i]);
  }
  mFitnessChart->show();
  /*int graph_width= mFitnessChart->size().width();

  int left_chart_space=66;
  int right_chart_space=44;

  int position_line=422;
  if(graph_width!=0)
  {
    position_line = left_chart_space + (graph_width-left_chart_space-right_chart_space)* ui->box_trait_optimum->value();
  }
  //Put the actual value line
  mActualValueLine->setGeometry(
    QRect(
      position_line, //Dirty hack
      80,
      3,
      300
     )
  );*/
}

void MainWindow::ShowNumberOfSeedsGraph(int percentage_facilitated, int percentage_unfacilitated, float average_trait_value_facilitated)
{
  //Dirty hack: just assume there are plants of all traits, from 0-1, both
  //facilitated and unfacilitated
  const double fitness_facilitated = normal(
    average_trait_value_facilitated,//ui->box_trait_optimum->value(),
    ui->box_fit_fac_opt->value(),
    ui->box_fit_fac_sd->value()
  );
  //std::cout<<"average: "<<average_trait_value_facilitated << "/n";
  const double fitness_unfacilitated = normal(
    0.50,//ui->box_trait_optimum->value(),
    ui->box_fit_unfac_opt->value(),
    ui->box_fit_unfac_sd->value()
  );

  mNumberOfSeedsFacilitatedSet->remove(0);
  mNumberOfSeedsFacilitatedSet->append(percentage_facilitated* fitness_facilitated);
  mNumberOfSeedsUnfacilitatedSet->remove(0);
  mNumberOfSeedsUnfacilitatedSet->append(percentage_unfacilitated* fitness_unfacilitated);
  mNumberOfSeedsChart->show();

  //ui->stackedWidget->addWidget(mNumberOfSeedsChartView);

}
void MainWindow::calculate_curent_trait_distribution()
{
    float trait_value = 0;
    current_trait_distribution.clear();
    for(int i = 0; i < 21; ++i)
    {
        current_trait_distribution.push_back(0);
    }
    for(int i = 0; i < 21; ++i)
    {
        for(unsigned j = 0; j < plant_trait_values.size(); ++j)
        {
            if(trait_value!=0 && trait_value!=1)
            {
                if(plant_trait_values[j]<=trait_value)
                {
                    if(plant_trait_values[j]>=trait_value-0.05)
                    {
                        current_trait_distribution[i] = current_trait_distribution[i] + 1;
                    }
                }
            }
            else
            {
                if(plant_trait_values[j]<0)
                {
                    current_trait_distribution[i] = current_trait_distribution[i] + 1;
                }

            }
            if(trait_value==1)
            {
                if(plant_trait_values[j]>trait_value)
                {
                    current_trait_distribution[i] = current_trait_distribution[i] + 1;
                }
            }
    }
        trait_value += 0.05;

}

}

void MainWindow::ShowCurrentTraitDistributionGraph()
{

    //if(current_trait_distribution.size() == 0 || ui->spinBox_init_n_seeds->value() < prev_plant_trait_size)
    //{
        /*current_trait_distribution.clear();
        for(int i = 0; i < 21; ++i)
        {
            current_trait_distribution.push_back(0);
        }*/
    //}

    prev_plant_trait_size = ui->spinBox_init_n_seeds->value();

    mCurrentTraitDistributionChart->removeAllSeries();
    QBarSeries *series = new QBarSeries();
    CurrentTraitdistributionSets = new QBarSet("Traits");
    calculate_curent_trait_distribution();
    *CurrentTraitdistributionSets << current_trait_distribution[0] << current_trait_distribution[1] << current_trait_distribution[2] << current_trait_distribution[3] << current_trait_distribution[4] << current_trait_distribution[5] << current_trait_distribution[6] << current_trait_distribution[7]
            << current_trait_distribution[8] << current_trait_distribution[9] << current_trait_distribution[10] << current_trait_distribution[11] << current_trait_distribution[12] << current_trait_distribution[13] << current_trait_distribution[14] << current_trait_distribution[15]
            << current_trait_distribution[16] << current_trait_distribution[17] << current_trait_distribution[18] << current_trait_distribution[19] <<current_trait_distribution[20];
    series->append(CurrentTraitdistributionSets);
    mCurrentTraitDistributionChart->addSeries(series);
    mCurrentTraitDistributionChart->show();
    //ui->stackedWidget->setWidget(mCurrentTraitDistributionView);
}
void MainWindow::GenerateGeneration(yx_grid& g, plant_coordinats &nurse_plant)
{
     nurse_plants_seeds(nurse_plant, g);
    //produce new plants
    //remove dead plants
}
void MainWindow::setPopulationSize(double value)
{
    if(ui->spinBox_init_n_seeds->value() != 0)
    {
        value = value * ui->spinBox_init_n_seeds->value();
    }
}
//button slots
//-------------------

//mutation rate
void MainWindow::on_spinBoxMutation_valueChanged(double arg1)
{
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
    mGeneration = value;
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
    //mNumberOfSeedsChartView->setHidden(true);
    //mCurrentTraitDistributionView->setHidden(true);
    CreateGrid();

}
//close grid
void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setHidden(false);
    ui->pushButton_7->setHidden(true);
    //ui->tableWidget->setHidden(true);
    //mNumberOfSeedsChartView->setHidden(false);
    //mCurrentTraitDistributionView->setHidden(false);
    RemoveGrid();
}

//run
void MainWindow::on_pushButton_run_pressed()
{
    RemoveGrid();
    generation_coordinates.clear();

}
//run
void MainWindow::on_pushButton_run_released()
{
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
    ui->spinBox_init_n_seeds->setValue(0);
    ui->spinBox_generation->setValue(0);
    ui->spinBox_n_nurse->setValue(0);
    //ui->box_trait_optimum->setValue(0);
    delay();
}

///-----------------------------------------------------------------------------
///RJCB put his functions here sorted alphabetically,
/// as there appeared to be no order in the place of functions
///-----------------------------------------------------------------------------

void MainWindow::ShowGraphs()
{
    ShowFitnessGraph();
    //ShowNumberOfSeedsGraph();
    //ShowCurrentTraitDistributionGraph();
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
