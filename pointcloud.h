#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QGLWidget>
#include <QtOpenGL>
#include <QtWidgets>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <GL/glu.h>
#include<QMatrix4x4>

class PointCloud : public QOpenGLWidget
{
    Q_OBJECT
public:
    PointCloud(QWidget *parent =0);
    ~PointCloud();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXParallel(int angle);
    void setYParallel(int angle);
    void setZParallel(int angle);
    void draw_axis();
    void draw();
    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void read_data();

   // QMatrix4x4
    int xRot;
    int yRot;
    int zRot;
    int xPal;
    int yPal;
    int zPal;

    QPoint lastPos;
    GLdouble vertex[25000*10][3];
    double hmax, hmin, lrf_height;
    int maxpoint;
    const double RGB[3][60] = {
        { 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 0.933333, 0.866667, 0.800000, 0.733333, 0.666667, 0.600000, 0.533333, 0.466667, 0.400000, 0.333333, 0.266667, 0.200000, 0.133333, 0.066667, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 },
        { 0.000000, 0.066667, 0.133333, 0.200000, 0.266667, 0.333333, 0.400000, 0.466667, 0.533333, 0.600000, 0.666667, 0.733333, 0.800000, 0.866667, 0.933333, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 0.933333, 0.866667, 0.800000, 0.733333, 0.666667, 0.600000, 0.533333, 0.466667, 0.400000, 0.333333, 0.266667, 0.200000, 0.133333, 0.066667 },
        { 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.066667, 0.133333, 0.200000, 0.266667, 0.333333, 0.400000, 0.466667, 0.533333, 0.600000, 0.666667, 0.733333, 0.800000, 0.866667, 0.933333, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000 } };
};

#endif // POINTCLOUD_H
