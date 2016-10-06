#include "righteye.h"

RightEye::RightEye(QWidget *parent) : QGraphicsView(parent)
{
    mQImage.load("../mario.png");
    mQImage.convertToFormat(QImage::Format_ARGB32);
    setAlpha(128);
    mRect = QRect(0, 0, mQImage.width(), mQImage.height());
}

void RightEye::paintEvent(QPaintEvent *)
{
  QPainter qPainter(viewport());
  QRect rect(0, 0, width(), height());
  qPainter.drawImage(rect, mQImage, mRect);
}

void RightEye::setAlpha(int alpha)
{
  for (int x = 0, width = mQImage.width(); x < width; ++x)
    for (int y = 0, height = mQImage.height(); y < height; ++y) {
      QColor color(mQImage.pixel(x, y));
      color.setAlpha(alpha);
      mQImage.setPixel(x, y, color.rgba());
    }
}
