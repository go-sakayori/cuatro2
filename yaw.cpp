#include "yaw.h"

yaw::yaw(QWidget *parent) : QOpenGLWidget(parent)
{
    float yaw_angle=0.0*M_PI/180;

}

yaw::~yaw()
{


}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}


void yaw::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

}

void yaw::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = width / (height ? height : 1);

    const GLdouble zNear = 1.0, zFar = 100.0, fov = 10.0;

    perspective(fov, aspect, zNear, zFar);
    //read_angle();

    glMatrixMode(GL_MODELVIEW);
}

void yaw::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-2.0, -2.0, -15.0);
    draw_yaw();
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

}

void yaw::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void yaw::draw_yaw()
{
    float camera_headx,camera_heady;
    Rover_3D r3;
    camera_headx = cos(yaw_angle);
    camera_heady = sin(yaw_angle);
    gluLookAt(-2,2,1,-2,2,0,1,0,0);
    r3.draw_rover_3D();

}
void yaw::read_angle()
{
    int i;
    for(i=0;i<60;i++)
    {
        //yaw_angle=-1*i*M_PI/180;
    }

}
