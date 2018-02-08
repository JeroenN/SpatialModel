/*#include "grid.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>


grid::grid(
  const int width,
  const int height,
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtMutualismBreakdownerSpatialWidget),
    m_image(width,height,QImage::Format_RGB32)
{
  ui->setupUi(this);
}

~Grid::Grid()
{
  delete ui;
}

void SetPixel(const int x, const int y, const QColor color)
{
  m_image.setPixel(x,y,color.rgb());
}

void SetResolution(const int width, const int height)
{
  m_image = QImage(width,height,QImage::Format_RGB32);
}

void paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    QPixmap::fromImage(m_image)
  );
}*/
