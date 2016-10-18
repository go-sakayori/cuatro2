#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QMainWindow>
#include "pointcloud.h"
#include "urgfile.h"
#include <stdio.h>
namespace Ui {
class SubWindow;
}

class SubWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubWindow(QWidget *parent = 0);
    ~SubWindow();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::SubWindow *ui;
};

#endif // SUBWINDOW_H
