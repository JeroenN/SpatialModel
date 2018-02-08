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
