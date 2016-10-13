#include "pointcloud.h"

#include <QMouseEvent>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <fstream>

PointCloud::PointCloud(QWidget *parent)
    : QOpenGLWidget(parent)
{
    alpha = 25;
    beta = -25;
    distance = 2.5;
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
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
    glMultMatrixf(cv.lookat.data());

//    QMatrix4x4 vMatrix;
//    vMatrix.setToIdentity();

//    QMatrix4x4 cameraTransformation;
//    cameraTransformation.rotate(alpha, 0, 1, 0);
//    cameraTransformation.rotate(beta, 1, 0, 0);
//    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
//    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

//    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
//    glMultMatrixf(vMatrix.data());

    draw_axis();
    draw();

}

void PointCloud::mousePressEvent(QMouseEvent *event)
{
    cv.mousePressPosition = QVector2D(event->localPos());
}

void PointCloud::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D diff = QVector2D(event->localPos()) - cv.mousePressPosition;
    if (event->buttons() & Qt::LeftButton) {
       cv.phi -= 2 * diff.x() / width();
       cv.theta += 2 * diff.y() / height();
    }
    else if (event->buttons() & Qt::RightButton) {
    cv.eye += cv.left * diff.x() / (0.025 * width()) + cv.up * diff.y() / (0.025 * height());
    }

    update_variables();
    cv.mousePressPosition = QVector2D(event->localPos());
    update();
}

void PointCloud::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
       {
           int delta = event->delta() / 4;
           cv.eye += delta * cv.forward / 10.0;
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
    std::string file = "/home/go/Qt5.7.0/data3D.txt";
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

        vertex[i][0] = std::atof(v[0].c_str())/1000;
        vertex[i][1] = (std::atof(v[1].c_str()) )/1000;
        vertex[i][2] = std::atof(v[2].c_str())/1000 + lrf_height;

        htmp = vertex[i][2];
        if(htmp > hmax)
            hmax = htmp;
        if(htmp < hmin)
            hmin = htmp;
        i++;
    }
    maxpoint = i;
    terrain.close();
}

void PointCloud::update_variables()
{
    cv.forward.setZ(cv.r * sin(cv.theta) * cos(cv.phi));
    cv.forward.setX(cv.r * sin(cv.theta) * sin(cv.phi));
    cv.forward.setY(cv.r * cos(cv.theta));
    cv.forward.normalize();

    cv.up.setZ(cv.r * sin(cv.theta-M_PI_2) * cos(cv.phi));
    cv.up.setX(cv.r * sin(cv.theta-M_PI_2) * sin(cv.phi));
    cv.up.setY(cv.r * cos(cv.theta-M_PI_2));
    cv.up.normalize();

    cv.center = cv.eye + cv.forward;

    cv.left = cv.left.crossProduct(cv.up, cv.forward);

    cv.lookat.setToIdentity();
    cv.lookat.lookAt(cv.eye,cv.center,cv.up);
}

PointCloud::ControlVariables::ControlVariables()
{
    //initialize lookat Variables
    up = QVector3D(0, 0, 0);
    center = QVector3D(0, 0, 0);
    eye = QVector3D(-3, 0, 0);
    r = eye.length();
    theta = acos(-eye.y() /r );
    phi = atan2(-eye.x(), -eye.z());
}
