#include "pitch.h"


pitch::pitch(QWidget *parent) : QOpenGLWidget(parent)
{
    float pitch_angle=0.0;
}

pitch::~pitch()
{


}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}


void pitch::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

}

void pitch::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = width / (height ? height : 1);

    const GLdouble zNear = 1.0, zFar = 100.0, fov = 10.0;

    perspective(fov, aspect, zNear, zFar);


    glMatrixMode(GL_MODELVIEW);
}

void pitch::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-2.0, -2.0, -15.0);
    draw_pitch(pitch_angle);
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

}

void pitch::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void pitch::draw_pitch(float pitch_angle)
{
    Rover_3D r3;
    gluLookAt(-2,-1,-1,-2,0,-1,0,0,1);
    r3.draw_rover_3D();



}

