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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBoxMutation_valueChanged(double arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void setProportionratio();
    void CreateGraph();
    void CreateGrid();
    void DestroyGraph();

    void on_proportionRatioUF_valueChanged(double arg1);
    void setPopulationSize(double value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_spinBox_3_valueChanged(int arg1);

    void on_pushButton_5_clicked();

    void on_doubleSpinBox_7_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_doubleSpinBox_5_valueChanged(double arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

 private:
    Ui::MainWindow *ui;
    Explanation *explanation;
    QChart *chart;
    QGraphicsScene *scene;
    QLineSeries *fSeries;
    QLineSeries *uFSeries;
    QChartView *chartView;

    QGraphicsScene *gridScene;
    QQuickWidget *gridWidget;

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
    std::vector<float> fXAppends;
    std::vector<float> fYAppends;
    std::vector<float> uFXAppends;
    std::vector<float> uFYAppends;

};

#endif // MAINWINDOW_H
