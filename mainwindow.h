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

///RJCB: Make usings global (one day you'll put these in a namespace)
using yx_grid = std::vector<std::vector<QColor>>;
using coordinat = std::pair<int,int>; //RJCB: renamed to 'coordinat', from erroneous 'coordinate'
using plant_coordinats = std::vector<coordinat>; //RJCB: renamed to 'plant_coordinats', from erroneous 'plant_coordinates'
using generations = std::vector<plant_coordinats>;

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

    //RJCB: Does not do anything useful
    //void on_spinBox_n_f_valueChanged(int arg1);

    //RJCB: Does not do anything useful
    //void on_spinBox_n_uf_valueChanged(int arg1);

    void ShowFitnessGraph();
    void CreateGrid();
    void RemoveGrid();

    ///Show how the current parameters have an effect on
    ///the starting grid and graphs
    void ShowStart();

    //void SetGridResolution();
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

    //RJCB: Removed mNnursePlants to avoid data duplication
    //void on_spinBox_n_nurse_valueChanged(int arg1);

    void on_pushButton_run_pressed();

    void on_pushButton_run_released();

    void on_pushButton_clicked();



 private:
    Ui::MainWindow *ui;
    Explanation *explanation;
    QChart *mChart;
    QGraphicsScene *mScene;
    QLineSeries *mSeriesFacilitated;
    QLineSeries *mSeriesUnfacilitated;
    QChartView *mChartView;
    QChartView *usingChartView;

    QGraphicsScene *gridScene;
    QQuickWidget *gridWidget;

    //int initialPopulationSizeF; //RJCB: Made this an int, was a float
    //int initialPopulationSizeUF; //RJCB: Made this an int, was a float
    //float proportionRatio; //RJCB: Unused, thus removed
    float mutationRate;
    float h2OGroundChange;

    ///The number of nurse plants
    //int mNnursePlants; //RJCB: Made this an int, was a float
    int mGeneration = 0;
    //int mRngSeed; //RJCB: Just read from GUI
    float temperatureChange;

    //grid
    //std::vector<std::vector<int> > theGrid; //RJCB: You didn't use this one

    QImage m_image;
    const QColor green = QColor(57,188,31);
    const QColor brown = QColor(149,113,39);
    const QColor blue = QColor(56,119,182);
    //QColor usingColor = QColor(0,0,0); //RJCB: Only added state, without value
    const QColor white =  QColor(239,235,231);
    const QColor red = QColor(255,0,0);
    int gridXStart;
    int gridXEnd;
    int gridYStart;
    int gridYEnd;

    ///The history of all plants
    generations generation_coordinates;

    //yx_grid g; //RJCB: Unused, so removed

    ///Clear the GUI grid
    /// Note: only using g for size :-(
    //void ClearGrid(const yx_grid& g);

    void DrawGrid(const yx_grid& g);

    //RJCB: Refactored (read: simplified)
    //void distance_between_nurse_plants(plant_coordinats &nurse_plant, int &add_nurse_plant_i, yx_grid& g);

    void add_nurse_plants(yx_grid& g, plant_coordinats &nurse_plant);
    void position_check_facilitated_plant(int position_difference_x, int position_difference_y, bool &make_facilitated_plant,
                                          bool &make_unfacilitated_plant, bool &stop_unfaciliated, bool &stop_faciliated);

    void position_in_relation_to_plants(
        yx_grid& g,
        const plant_coordinats& nurse_plant_coordinats,
        const plant_coordinats& seed_coordinats,
        plant_coordinats &facilitated_plant,
        plant_coordinats &unfacilitated_plant,
        const coordinat& c,
        int &n_facilitated_plant,
        int &n_unfacilitated_plant
    );
    void set_plants(yx_grid& g);
    void set_facilitated_and_unfacilitated_plants(yx_grid& g, plant_coordinats &nurse_plant, plant_coordinats &facilitated_plant, plant_coordinats &unfacilitated_plant);
    bool distance_between_facilitated_plants(const plant_coordinats facilitated_plant);
    bool distance_between_unfacilitated_plants(const plant_coordinats unfacilitated_plant);
    void GenerateGeneration(yx_grid& g, plant_coordinats &nurse_plant);
    void save_generation(plant_coordinats &nurse_plant);
    void set_seed_nurse_plant_coordinates(int x, int y, int &nurse_plant_x, int &nurse_plant_y, plant_coordinats nurse_plant, int i);
    void check_seed_nurse_plant_coordinates(int x, int y, int &nurse_plant_x, int &nurse_plant_y, plant_coordinats &nurse_plant, int i, plant_coordinats seed_coordinate, yx_grid& g);
    void new_generation(plant_coordinats seed_coordinate, plant_coordinats &nurse_plant, yx_grid& g);
    void nurse_plants_seeds(plant_coordinats &nurse_plant, yx_grid& g);




    //void set_seed(); //RJCB: Too simple to keep in

    //void DrawGrid2(); //RJCB: Unused code should be deleted
};

yx_grid create_vector_grid(int n_rows, int n_columns, QColor color);

/// Return a normal distribution (with mean zero and standard
/// deviation of sd) density at x. Will be heighest at x equals zero
double gauss(const double x, const double sd) noexcept;

/// Return a normal distribution (with a mean value at mean and standard
/// deviation of sd) at x. Will be heighest at x equals mean
double normal(const double x, const double mean, const double sd) noexcept;

///See if coordinat c would be a good spot for a nurse plant.
///Will be false if c is too close to another nurse plant
/// @param c the spot to see if it would be spot where a nurse plant can be put
/// @param nurse_plant_coordinats coordinats of nurse plants already present
/// @param min_dx minimal number of horizontal squares between two nurse plants.
///   A min_dx of zero denotes that adjacent nurse plants are allowed
/// @param min_dy minimal number of vertical squares between two nurse plants.
///   A min_dy of zero denotes that adjacent nurse plants are allowed
bool is_good_spot_for_nurse_plant(
  const coordinat& c,
  const plant_coordinats& nurse_plant_coordinats,
  const int min_dx = 1,
  const int min_dy = 1
);

#endif // MAINWINDOW_H
