#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    subui =new SubWindow(this);
    subui->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
