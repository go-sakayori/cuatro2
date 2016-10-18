/*
Code to display point cloud earned by LRF
Fist Version: 13 Oct. 2016 by  G.Sakayori
Last Modified: 13 Oct. 2016 by G.Sakayori
*/

#include "pointcloud.h"

#include <QMouseEvent>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <fstream>

PointCloud::PointCloud(QWidget *parent)
    : QOpenGLWidget(parent)
{
    hmax = -10;
    hmin = 1000000;
    lrf_height = 0.0;
    read_data();

}

PointCloud::~PointCloud()
{

}

void PointCloud::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.1f); //specify clear values for the color buffer
    glEnable(GL_DEPTH_TEST); //enable depth buffer
}

void PointCloud::resizeGL(int width, int height)
{
    qreal aspect = qreal(width) / qreal(height ? height : 1); //calculate aspect ratio
    const qreal zNear = 0.01, zFar = 100.0, fov = 30.0; // set near plane, far plane, field of view

    cv.perspective.setToIdentity(); //reset projection
    cv.perspective.perspective(fov, aspect, zNear, zFar); //set perspective projection

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(cv.perspective.data());

    glViewport(0, 0, width, height);
}

void PointCloud::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(cv.lookat.data()); //set position of camera

    draw_axis();
    draw(); //draws the point cloud data
}

void PointCloud::mousePressEvent(QMouseEvent *event)
{
    cv.mousePressPosition = QVector2D(event->localPos());
}

void PointCloud::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D(event->localPos()) - cv.mousePressPosition;
    if (event->buttons() & Qt::LeftButton) {
        cv.phi += 0.02 * diff.x();
        cv.theta += 0.02 * diff.y();
        update_variables();
    }
    else if (event->buttons() & Qt::RightButton) {
        double x_tmp = cv.center.x() + diff.y() * 0.02;
        double y_tmp = cv.center.y() - diff.x() * 0.02;
        cv.center.setX(x_tmp);
        cv.center.setY(y_tmp);
        cv.center.setZ(0.0);
        update_variables();
    }

    cv.mousePressPosition = QVector2D(event->localPos());
}

void PointCloud::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
       {
           int delta = event->delta() / 4;
           cv.r += delta * 0.02;
           update_variables();
           update();
       }
}


void PointCloud::draw_axis()
{
  glLineWidth(2.0);
  glBegin(GL_LINES);

  //x axis
  glColor3d(1.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0 , 0.0);

  //y axis
  glColor3d(0.0, 1.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);

  //z axis
  glColor3d(0.0, 0.0, 1.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 1.0);

  glEnd();
}

void PointCloud::draw()
{
    double a = (hmax - hmin) / 60;
    glPointSize(1.0);
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);

    for (int k = 0; k < maxpoint; k++)
    {
        int rgbID = (int)((hmax - vertex[k][2]) / a);
        glColor4d(RGB[0][rgbID], RGB[1][rgbID], RGB[2][rgbID], 1.0);
        glVertex3dv(vertex[k]);
    }
    glEnd();
}


void PointCloud::read_data()
{
    int i=0;
    double htmp = 0.0;
    std::string str;
    std::string file = "../data3D.txt";
    std::ifstream terrain(file, std::ios::in);

    if(!terrain)
    {
        std::cout<<"Point Cloud Data not read"<<std::endl;
        exit(1);
    }

    while(std::getline(terrain, str))
    {
        std::vector<std::string> v;
        boost::algorithm::split(v, str, boost::is_any_of("\t"));

        double x_tmp = std::atof(v[0].c_str()) / 1000;
        double y_tmp = std::atof(v[1].c_str()) / 1000;
        double z_tmp = std::atof(v[2].c_str()) / 1000 + lrf_height;
        double r_tmp = sqrt(x_tmp * x_tmp + y_tmp * y_tmp);

        if(r_tmp < 15.0 && z_tmp < 3.0){
            vertex[i][0] = x_tmp;
            vertex[i][1] = y_tmp;
            vertex[i][2] = z_tmp;

            htmp = vertex[i][2];
            if(htmp > hmax)
                hmax = htmp;
            if(htmp < hmin)
                hmin = htmp;
            i++;
        }
    }
    maxpoint = i;
    terrain.close();
}

void PointCloud::update_variables()
{
    cv.forward.setX(cv.r * sin(cv.theta) * cos(cv.phi));
    cv.forward.setY(cv.r * sin(cv.theta) * sin(cv.phi));
    cv.forward.setZ(cv.r * cos(cv.theta));

    cv.eye = cv.center + cv.forward;

    cv.up = QVector3D(0, 0, 1);

    cv.lookat.setToIdentity();
    cv.lookat.lookAt(cv.eye, cv.center, cv.up);
    update();
}

PointCloud::ControlVariables::ControlVariables()
{
    //initialize lookat Variables
    up = QVector3D(0, 0, 1);
    center = QVector3D(1, 0, 0);
    eye = QVector3D(-5, 0, 2);
    r = eye.length();
    theta = acos(eye.z() /r );
    phi = atan2(eye.y(), eye.x());

    lookat.setToIdentity();
    lookat.lookAt(eye, center, up);
}
