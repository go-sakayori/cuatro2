#ifndef LEFTEYE_H
#define LEFTEYE_H

#include <QGraphicsView>
#include <QImage>
#include <QRect>
#include <QWidget>
#include <iostream>

class LeftEye : public QGraphicsView
{
public:
    LeftEye(QWidget *parent =0);
    void paintEvent(QPaintEvent *);

private:
    QImage mQImage;
    QRect mRect;
    void setAlpha(int alpha);
};

#endif // LEFTEYE_H
