#ifndef RIGHTEYE_H
#define RIGHTEYE_H

#include <QGraphicsView>
#include <QImage>
#include <QRect>
#include <QWidget>
#include <iostream>

class RightEye : public QGraphicsView
{
public:
    RightEye(QWidget *parent =0);
    void paintEvent(QPaintEvent *);

private:
    QImage mQImage;
    QRect mRect;
    void setAlpha(int alpha);
};

#endif // RIGHTEYE_H
