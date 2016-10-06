#include "pointcloud.h"

PointCloud::PointCloud(QWidget *parent)
    : QOpenGLWidget(parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    xPal = 8;
    yPal = 2;
    zPal = 0;
    hmax = -10;
    hmin = 1000000;
    lrf_height = 0.0;
    std::vector<std::vector<double>> vertex;
    read_data();

}

PointCloud::~PointCloud()
{

}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void PointCloud::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void PointCloud::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-2.0, -2.0, -15.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    gluLookAt(xPal,yPal,30,xPal,yPal,0,1,0,0);
    draw_axis();
    draw();
}

void PointCloud::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);   
      //プロジェクション・マトリックス設定
      glMatrixMode (GL_PROJECTION);
//      glMatrixMode (GL_MODELVIEW);
//      glLoadIdentity();
//      QMatrix4x4 m;
//      m.lookAt(QVector3D(3.0, 4.0, 5.0),
//                 QVector3D(0.0, 0.0, 0.0),
//                 QVector3D(0.0, 1.0, 0.0) );

//      m.perspective(30, (qreal)width / (qreal)height, 1.0f, 100.0f);
//      glMultMatrixd(m.data());
//    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = width / (height ? height : 1);

    const GLdouble zNear = 1.0, zFar = 100.0, fov = 30.0;

    perspective(fov, aspect, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
}

void PointCloud::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void PointCloud::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 4 * dy);
        setYRotation(yRot + 4 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXParallel(xPal + 1 * dy);
        setYParallel(yPal + 1 * dx);
    }

    lastPos = event->pos();
}

void PointCloud::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        update();
    }

}
void PointCloud::setXParallel(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xPal) {
        xPal = angle;
        update();
    }
}

void PointCloud::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        update();
    }
}

void PointCloud::setYParallel(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yPal) {
        yPal = angle;
        update();
    }
}


void PointCloud::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        update();
    }
}
void PointCloud::setZParallel(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zPal) {
        zPal = angle;
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

void PointCloud::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
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
