#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "explanation.h"
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QQuickWidget>
#include <QColor>

//grid
#include <QWidget>
#include <QImage>
#include <stdlib.h>

using namespace QtCharts;


namespace Ui {
class MainWindow;
}

 class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_spinBoxMutation_valueChanged(double arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void CreateGraph();
    void CreateGrid();
    void RemoveGrid();
    void DrawGrid2();
    //void SetGridResolution();
    void set_seed();
    void delay();


    //void CheckColorGrid(int x, int y, QColor color);
    void SetPixel(const int x, const int y, const QColor color);

    void SetResolution(const int width, const int height);
    void paintEvent(QPaintEvent *);

    void on_proportionRatioUF_valueChanged(double arg1);
    void setPopulationSize(double value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_spinBox_3_valueChanged(int arg1);

    void on_pushButton_5_clicked();

    void on_doubleSpinBox_7_valueChanged(double arg1);

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

 private slots:


    void on_spinBox_5_valueChanged(int arg1);

    void on_pushButton_3_pressed();

    void on_pushButton_3_released();

    void on_pushButton_clicked();



 private:
    Ui::MainWindow *ui;
    Explanation *explanation;
    QChart *chart;
    QGraphicsScene *scene;
    QLineSeries *fSeries;
    QLineSeries *uFSeries;
    QChartView *chartView;
    QChartView *usingChartView;

    QGraphicsScene *gridScene;
    QQuickWidget *gridWidget;

    float initialPopulationSizeF;
    float initialPopulationSizeUF;
    float proportionRatio;
    float mutationRate;
    float h2OGroundChange;
    float nursePlants;
    int generation = 0;
    int rngSeed;
    float temperatureChange;
    std::vector<float> fXAppends;
    std::vector<float> fYAppends;
    std::vector<float> uFXAppends;
    std::vector<float> uFYAppends;

    //grid
    std::vector<std::vector<int> > theGrid;

    QImage m_image;
    QColor green = QColor(57,188,31);
    QColor brown = QColor(149,113,39);
    QColor blue = QColor(56,119,182);
    QColor usingColor = QColor(0,0,0);
    QColor white =  QColor(239,235,231);
    QColor red = QColor(255,0,0);
    int gridXStart;
    int gridXEnd;
    int gridYStart;
    int gridYEnd;

    using yx_grid = std::vector<std::vector<QColor>>;
    using coordinate = std::pair<int,int>;
    using plant_coordinates = std::vector<coordinate>;
    using generations = std::vector<plant_coordinates>;
    generations generation_coordinates;
    yx_grid g;
    yx_grid create_vector_grid(int n_rows, int n_columns)
    {
       yx_grid grid;
       std::vector<QColor> column;

       for(int i=0; i<n_columns; ++i)
       {
            if(i == 0)
            {
                usingColor = brown;
            }
           column.push_back(usingColor);
       }
       for(int i=0; i<n_rows; ++i)
       {
           grid.push_back(column);
       }

       return grid;
    }

    void DrawGrid(const yx_grid& g);
    void distance_between_nurse_plants(plant_coordinates &nurse_plant, int &add_nurse_plant_i, yx_grid& g);
    void set_nurse_plant(yx_grid& g, plant_coordinates &nurse_plant);
    void position_check_facilitated_plant(int position_difference_x, int position_difference_y, bool &make_facilitated_plant,
                                          bool &make_unfacilitated_plant, bool &stop_unfaciliated, bool &stop_faciliated);

    void position_in_relation_to_plants(yx_grid& g, plant_coordinates nurse_plant, plant_coordinates seeds, int &position_difference_x, int &position_difference_y,
                                        plant_coordinates &facilitated_plant, plant_coordinates &unfacilitated_plant, coordinate c, int &n_facilitated_plant
                                        , int &n_unfacilitated_plant);
    void set_plants(yx_grid& g);
    void set_facilitated_and_unfacilitated_plants(yx_grid& g, plant_coordinates &nurse_plant, plant_coordinates &facilitated_plant, plant_coordinates &unfacilitated_plant);
    bool distance_between_facilitated_plants(const plant_coordinates facilitated_plant);
    bool distance_between_unfacilitated_plants(const plant_coordinates unfacilitated_plant);
    void GenerateGeneration(yx_grid& g, plant_coordinates &nurse_plant);
    void save_generation(plant_coordinates &nurse_plant);
    void set_seed_nurse_plant_coordinates(int x, int y, int &nurse_plant_x, int &nurse_plant_y, plant_coordinates nurse_plant, int i);
    void check_seed_nurse_plant_coordinates(int x, int y, int &nurse_plant_x, int &nurse_plant_y, plant_coordinates &nurse_plant, int i, plant_coordinates seed_coordinate, yx_grid& g);
    void new_generation(plant_coordinates seed_coordinate, plant_coordinates &nurse_plant, yx_grid& g);
    void nurse_plants_seeds(plant_coordinates &nurse_plant, yx_grid& g);







};

#endif // MAINWINDOW_H
