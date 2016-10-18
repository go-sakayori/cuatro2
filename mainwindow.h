#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "subwindow.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


private:
    Ui::MainWindow *ui;
    SubWindow *subui;

};

#endif // MAINWINDOW_H
