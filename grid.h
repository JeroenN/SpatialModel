/*#ifndef GRID_H
#define GRID_H

#include <QWidget>
#include <QImage>

/*namespace Ui {
  class QtMutualismBreakdownerSpatialWidget;
}

struct QImage;

namespace ribi {
namespace mb {

class Grid : public QWidget
{
  Q_OBJECT

public:
  explicit Grid(
    const int width = 600,
    const int height = 400,
    QWidget *parent = 0
  );
  Grid(const Grid&) = delete;
  Grid& operator=(const Grid&) = delete;
  ~Grid();

  void SetPixel(const int x, const int y, const QColor color);
  void SetResolution(const int width, const int height);

protected:
  void paintEvent(QPaintEvent *);
private:
  //Ui::Grid *ui;
  QImage m_image;
};

} //~namespace mb
} //~namespace ribi

#endif // GRID_H
*/

//current_trait_distribution.clear();
/*if(current_trait_distribution.size>10)
{
    for(int i = 0; i < 21; ++i)
    {
        current_trait_distribution.push_back(0);
        //int n_traits = 0;
        for(unsigned j = 0; j < plant_trait_values.size()-1; ++j)
        {
            if(trait_value!=0 && trait_value!=1)
            {
                if(plant_trait_values[j]<=trait_value)
                {
                    if(plant_trait_values[j]>=trait_value-0.05)
                    {
                        n_traits++;
                    }
                }
            }
            else
            {
                if(plant_trait_values[j]<0)
                {
                    n_traits++;
                }

            }
            if(trait_value==1)
            {
                if(plant_trait_values[j]>trait_value)
                {
                    n_traits++;
                }
            }*/
