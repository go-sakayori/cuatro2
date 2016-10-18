#include "subwindow.h"
#include "ui_subwindow.h"

SubWindow::SubWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubWindow)
{
    ui->setupUi(this);
}

SubWindow::~SubWindow()
{
    delete ui;
}

void SubWindow::on_horizontalSlider_sliderMoved(int position)
{
    UrgFile urg;
    urg.get_file_num(position);
    printf("pk\n");
}
