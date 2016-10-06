#include "rover_3d.h"

Rover_3D::Rover_3D(QWidget *parent) : QOpenGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
}


Rover_3D::~Rover_3D()
{

}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}


void Rover_3D::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
   
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

}

void Rover_3D::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = width / (height ? height : 1);

    const GLdouble zNear = 1.0, zFar = 100.0, fov = 30.0;

    perspective(fov, aspect, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
}

void Rover_3D::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-2.0, -2.0, -10.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw_rover_3D();
}

void Rover_3D::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void Rover_3D::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void Rover_3D::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }
}

void Rover_3D::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

void Rover_3D::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}

void Rover_3D::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}


void Rover_3D::draw_rover_3D()
{
    float BodyCenterH = 0.65;
    float Hoffset = BodyCenterH;
    int i;
    double j;
    double H = 0.1;
    double CG_H = BodyCenterH;
    double w_b2=0.10;
    double w_rad=0.11;
    double w_tread = 0.7/2.0;//W+0.15;
    double w_base  = 1.1/2.0;//L+0.25;

    GLdouble tmp0[3],tmp[3];

    GLdouble body[12][3]={
      { 0.20,   0.1, H + Hoffset},
      { 0.10,   0.2, H + Hoffset},
      {  -0.40,  0.2,  H + Hoffset},
      {  -0.40, -0.2,  H + Hoffset},
      { 0.10,   -0.2,H + Hoffset},
      { 0.20,   -0.1,H + Hoffset},
      { 0.20,   0.1, -H + Hoffset},
      { 0.10,   0.2, -H + Hoffset},
      {  -0.40,  0.2, -H + Hoffset},
      {  -0.40, -0.2,  -H + Hoffset},
      { 0.10,   -0.2,-H + Hoffset},
      { 0.20,   -0.1,-H + Hoffset}};

    GLdouble bodyline[8][3]={
      {  w_base,  w_tread+0.025, w_rad-CG_H + Hoffset},
      {  -0.15,     w_tread, 0 + Hoffset},
      { -w_base,  w_tread, w_rad-CG_H + Hoffset},
      {  -0.15,     w_tread, 0 + Hoffset},
      {  w_base, -w_tread-0.025, w_rad-CG_H + Hoffset},
      {  -0.15,    -w_tread, 0 + Hoffset},
      { -w_base, -w_tread, w_rad-CG_H + Hoffset},
      {  -0.15,    -w_tread, 0 + Hoffset}};

    GLdouble wheel0[4][3]={
      {  w_base,  w_tread+0.025, w_rad-CG_H + Hoffset},
      { -w_base,  w_tread, w_rad-CG_H + Hoffset},
      {  w_base, -w_tread, w_rad-CG_H + Hoffset},
      { -w_base, -w_tread-0.025, w_rad-CG_H + Hoffset}};

    GLdouble mast[2][3]={
      {  0.0, 0.0, H + Hoffset},
      {  0.0, 0.0, H + Hoffset+0.6}};

    GLdouble cam_stage[8][3]={
      {  0.03, 0.2, H + Hoffset+0.6},
      {  0.08, 0.2, H + Hoffset+0.6},
      {  0.08, -0.2, H + Hoffset+0.6},
      {  0.03, -0.2, H + Hoffset+0.6},
      {  0.03, 0.2, H + Hoffset+0.65},
      {  0.08, 0.2, H + Hoffset+0.65},
      {  0.08, -0.2, H + Hoffset+0.65},
      {  0.03, -0.2, H + Hoffset+0.65}};

    GLdouble urg_box[8][3]={
      {  0.06, -0.06, H+0.65 + Hoffset},
      {  0.18, -0.06, H+0.65 + Hoffset},
      {  0.18, 0.06, H+0.65 + Hoffset},
      {  0.06, 0.06, H+0.65 + Hoffset},
      {  0.06, -0.06, H+0.65+0.150 + Hoffset},
      {  0.18, -0.06, H+0.65+0.150 + Hoffset},
      {  0.18, 0.06, H+0.65+0.150 + Hoffset},
      {  0.06, 0.06, H+0.65+0.150 + Hoffset}};

    GLdouble SAP_plane[12][3]={
      { -0.40,  0.20, H + Hoffset},
      {  0.35,  0.20, H + Hoffset},
      {  0.35,  0.70, H + Hoffset},
      { -0.40,  0.70, H + Hoffset},
      { -0.40,  0.375, H + Hoffset},
      { -0.90,  0.375, H + Hoffset},
      { -0.90, -0.375, H + Hoffset},
      { -0.40, -0.375, H + Hoffset},
      { -0.40, -0.70, H + Hoffset},
      {  0.35, -0.70, H + Hoffset},
      {  0.35, -0.20, H + Hoffset},
      { -0.40, -0.20, H + Hoffset}};

//    if(DRAW_ROVER){
      glBegin(GL_POLYGON);
      glColor3d(1.0,1.0,0.0);//top
      glVertex3dv(body[0]);  glVertex3dv(body[1]);  glVertex3dv(body[2]);
      glVertex3dv(body[3]);  glVertex3dv(body[4]);  glVertex3dv(body[5]);
      glEnd();
      glBegin(GL_POLYGON);
      glColor3d(0.8,0.8,0.8);
      glVertex3dv(body[6]);  glVertex3dv(body[7]);  glVertex3dv(body[8]);
      glVertex3dv(body[9]);  glVertex3dv(body[10]);  glVertex3dv(body[11]);
      glEnd();

      glBegin(GL_POLYGON);
      glColor3d(1.0,1.0,0.0);
      glVertex3dv(body[0]);  glVertex3dv(body[1]);  glVertex3dv(body[2]);
      glVertex3dv(body[8]);  glVertex3dv(body[7]);  glVertex3dv(body[6]);
      glEnd();
      glColor3d(1.0,1.0,0.0);
      glVertex3dv(body[5]);  glVertex3dv(body[4]);    glVertex3dv(body[3]);
      glVertex3dv(body[9]);  glVertex3dv(body[10]);  glVertex3dv(body[11]);
      glEnd();

      glBegin(GL_QUADS);
      glColor3d(1.0,1.0,0.0);
      glVertex3dv(body[0]);  glVertex3dv(body[5]);
      glVertex3dv(body[11]); glVertex3dv(body[6]);
      glEnd();
      glBegin(GL_QUADS);
      glColor3d(1.0,1.0,0.0);
      glVertex3dv(body[2]);  glVertex3dv(body[3]);
      glVertex3dv(body[9]); glVertex3dv(body[8]);
      glEnd();

      glLineWidth(5.0);
      glBegin(GL_LINES);
      glColor3d(0.8,0.8,0.8);
      for(i=0;i<4;i++)
        glVertex3dv(bodyline[i]);
      glEnd();

      glLineWidth(5.0);
      glBegin(GL_LINES);
      glColor3d(0.8,0.8,0.8);
      for(i=4;i<8;i++)
        glVertex3dv(bodyline[i]);
      glEnd();

      glPointSize(5.0);
      glBegin(GL_POINTS);
      glColor3d(0.8,0.8,0.8);
      for(i=0;i<8;i++)
        glVertex3dv(bodyline[i]);
      glEnd();

      for(i=0;i<4;i++){
        glBegin(GL_TRIANGLE_STRIP);
        glColor3d(0.0,1.0,1.0);
        for(j=0.0;j<=2*M_PI;j+=2*M_PI/20.0){
      tmp0[0] = wheel0[i][0] + w_rad*cos(j);
      tmp0[1] = wheel0[i][1] + w_b2;
      tmp0[2] = wheel0[i][2] + w_rad*sin(j);
      glVertex3dv(tmp0);

      tmp0[0] = wheel0[i][0] + w_rad*cos(j+2*M_PI/20.0);
      tmp0[1] = wheel0[i][1] - w_b2;
      tmp0[2] = wheel0[i][2] + w_rad*sin(j+2*M_PI/20.0);
      glVertex3dv(tmp0);
        }
        glEnd();

        glBegin(GL_POLYGON);
        glColor3d(0.8,0.8,0.8);
        glVertex3dv(wheel0[i]);
        for(j=0.0;j<=2*M_PI;j+=2*M_PI/20.0){
      tmp0[0] = wheel0[i][0] + w_rad*cos(j);
      tmp0[1] = wheel0[i][1] + w_b2;
      tmp0[2] = wheel0[i][2] + w_rad*sin(j);
      glVertex3dv(tmp0);
        }
        glEnd();
        glBegin(GL_POLYGON);
        glColor3d(0.8,0.8,0.8);
        glVertex3dv(wheel0[i]);
        for(j=0.0;j<=2*M_PI;j+=2*M_PI/20.0){
      tmp0[0] = wheel0[i][0] + w_rad*cos(j);
      tmp0[1] = wheel0[i][1] - w_b2;
      tmp0[2] = wheel0[i][2] + w_rad*sin(j);
      glVertex3dv(tmp0);
        }
        glEnd();
      }

      //Mast
      glBegin(GL_TRIANGLE_STRIP);
      glColor3d(0.75,0.75,0.75);
      for(j=0.0;j<=2*M_PI;j+=2*M_PI/20.0){
        tmp0[0] = mast[0][0] + 0.05*cos(j);
        tmp0[1] = mast[0][1] + 0.05*sin(j);
        tmp0[2] = mast[0][2];
        glVertex3dv(tmp0);

        tmp0[0] = mast[1][0] + 0.05*cos(j);
        tmp0[1] = mast[1][1] + 0.05*sin(j);
        tmp0[2] = mast[1][2];
        glVertex3dv(tmp0);
      }
      glEnd();

      glBegin(GL_POLYGON);
      glColor3d(0.75,0.75,0.75);
      glVertex3dv(cam_stage[0]);  glVertex3dv(cam_stage[1]);  glVertex3dv(cam_stage[2]);
      glVertex3dv(cam_stage[3]);  glVertex3dv(cam_stage[7]);  glVertex3dv(cam_stage[6]);
      glVertex3dv(cam_stage[5]);  glVertex3dv(cam_stage[4]);
      glEnd();
      glBegin(GL_POLYGON);
      glColor3d(0.75,0.75,0.75);
      glVertex3dv(cam_stage[6]);  glVertex3dv(cam_stage[7]);  glVertex3dv(cam_stage[4]);
      glVertex3dv(cam_stage[5]);  glVertex3dv(cam_stage[1]);  glVertex3dv(cam_stage[0]);
      glVertex3dv(cam_stage[3]);  glVertex3dv(cam_stage[2]);
      glEnd();

      glBegin(GL_POLYGON);
      glColor3d(.0,.0,1.0);
      glVertex3dv(urg_box[0]);  glVertex3dv(urg_box[1]);  glVertex3dv(urg_box[2]);
      glVertex3dv(urg_box[3]);  glVertex3dv(urg_box[7]);  glVertex3dv(urg_box[6]);
      glVertex3dv(urg_box[5]);  glVertex3dv(urg_box[4]);
      glEnd();
      glBegin(GL_POLYGON);
      glColor3d(.0,.0,1.0);
      glVertex3dv(urg_box[6]);  glVertex3dv(urg_box[7]);  glVertex3dv(urg_box[4]);
      glVertex3dv(urg_box[5]);  glVertex3dv(urg_box[1]);  glVertex3dv(urg_box[0]);
      glVertex3dv(urg_box[3]);  glVertex3dv(urg_box[2]);
      glEnd();

      //glBegin(GL_POLYGON);
      glBegin(GL_QUADS);
      glColor3d(.0,.0,1.5);//top
      for(i=0;i<4;i++){
        glVertex3dv(SAP_plane[i]);
      }
      glEnd();
      glBegin(GL_QUADS);
      for(i=4;i<8;i++){
        glVertex3dv(SAP_plane[i]);
      }
      glEnd();
      glBegin(GL_QUADS);
      for(i=8;i<12;i++){
        glVertex3dv(SAP_plane[i]);
      }
      glEnd();

//    }

//    else{
//      glPointSize(8.0);
//      glColor3d(1.0,0.0,0.0);
//      glBegin(GL_POINTS);
//      tmp[0] = 0.0;
//      tmp[1] = 0.0;
//      tmp[2] = CG_H;
//      glVertex3dv(tmp);
//      glEnd();

}
