#include <ctime>
#include <windows.h>
#include <functional>

#include "nehewidget.h"  

static GLdouble vertex_list[16][3]={
    -1.5f,  1.0f,  1.0f,
    -1.5f, -1.0f,  1.0f,
    1.5f, -1.0f,  1.0f,
    1.5f,  1.0f,  1.0f,
    0.5f,  1.0f,  1.0f,
    0.5f,  0.0f,  1.0f,
    -0.5f,  0.0f,  1.0f,
    -0.5f,  1.0f,  1.0f,
    ///
    -1.5f,  1.0f,  -1.0f,
    -1.5f, -1.0f,  -1.0f,
    1.5f, -1.0f,  -1.0f,
    1.5f,  1.0f,  -1.0f,
    0.5f,  1.0f,  -1.0f,
    0.5f,  0.0f,  -1.0f,
    -0.5f,  0.0f,  -1.0f,
    -0.5f,  1.0f,  -1.0f,
};

static GLdouble color_list[8][3]={
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

static float front_mat_shininess[] = {60.0};
static float front_mat_specular[] = {0.2,0.2,0.2,1.0};
static float front_mat_diffuse[] = {0.5,0.5,0.28,1.0};
static float back_mat_shininess[] = {60.0};
static float back_mat_specular[] = {0.5,0.5,0.2,1.0};
static float back_mat_diffuse[] = {1.0,0.9,0.2,1.0};

NeHeWidget::NeHeWidget(QWidget *parent) :
    QGLWidget(parent),spin(0.0),speed(2),
    m_thread(),m_direction(X),m_Polygontype(true),m_blank(true)
{  
    connect(&m_thread, SIGNAL(refresh_GL()),this , SLOT(spinDisplay()));
}  
NeHeWidget::~NeHeWidget()  
{
    if(m_thread.isRunning())
        m_thread.exit();
}

void NeHeWidget::rotation()
{
    if(!m_thread.isRunning())
        m_thread.start();
}

void NeHeWidget::spinDisplay()
{
    srand((unsigned)time(0));
    float tmp_ = 0.01 *float(rand()%10) - 0.05;
    //    qDebug("tmp = %f, speed = %f",tmp_,speed);
    //    speed += tmp_;
    if(speed<0.8f)
        speed = 2.0f;
    if(speed>24.0f)
        speed = 20.0f;
    spin = spin + speed;
    if (spin > 360.0)
        spin = spin - 360.0;
    emit speedUpdate(int(speed*4));
    updateGL();
}

void NeHeWidget::run()
{
    spinDisplay();
}


void NeHeWidget::initializeGL()
{
    //Enable Shade smooth
    glShadeModel( GL_SMOOTH );
    // black background
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    // Set depth cache
    glClearDepth( 1.0 );
    // Enable depth testing
    glEnable( GL_DEPTH_TEST );
    //
    glDepthFunc( GL_LESS );

    glutInitDisplayMode(GLUT_RGB| GLUT_DOUBLE);
    //Tell the system to correct the Perspective
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //    glutIdleFunc(static_cast<void(*)()>(spinDisplay));

    ///light setting
    GLfloat light1_position[] = { 5, 6 , 4.0 ,0.0};
    GLfloat light1_ambient[]  = { 1, 1, 1, 0.1};
    GLfloat light1_diffuse[]  = { 1, 1, 1, 0.5};
    GLfloat light1_specular[] = { 1, 1, 1, 1};
    GLfloat spot_direction[]  = { -1.0, -1.0 ,-1.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);// environment
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);// 漫反射
    glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular); // 镜面反射
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION , spot_direction);

    glLightfv(GL_LIGHT2, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT3, GL_POSITION, light1_position);

    /*设置材质*/
    glMaterialfv(GL_FRONT,GL_DIFFUSE,front_mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,front_mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,front_mat_shininess);

    glMaterialfv(GL_BACK,GL_DIFFUSE,back_mat_diffuse);
    glMaterialfv(GL_BACK,GL_SPECULAR,back_mat_specular);
    glMaterialfv(GL_BACK,GL_SHININESS,back_mat_shininess);

}  
void CALLBACK PolyLine3DBegin(GLenum type)
{
    glBegin(type);
}

void CALLBACK PolyLine3DVertex ( GLdouble * vertex)
{
    const GLdouble *pointer = (GLdouble *) vertex;
    glVertex3dv(pointer);
}

void CALLBACK PolyLine3DEnd()
{
    glEnd();
}

GLUtesselator* tesser()
{
    GLUtesselator * tess;
    tess=gluNewTess();
    gluTessCallback(tess,GLU_TESS_BEGIN,(void (CALLBACK*)())&PolyLine3DBegin);
    gluTessCallback(tess,GLU_TESS_VERTEX,(void (CALLBACK*)())&PolyLine3DVertex);
    gluTessCallback(tess,GLU_TESS_END,(void (CALLBACK*)())&PolyLine3DEnd);
    return tess;
}


void NeHeWidget::paintGL()
{
    // Clear screen and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glScalef(1.3,1,1.3);
    glTranslatef(0.0f,0.0f,-12.0f);
    glColor3f( 1.0, 1.0, 1.0 );
    //Start Draw
    glPushMatrix();
    switch (m_direction) {
    case X:
        glRotatef(spin, 1.0, 0.0, 0.0);
        break;
    case Y:
        glRotatef(spin, 0.0, 1.0, 0.0);
        break;
    default:
        speed = 0;
        emit speedUpdate(int(speed*4));
        break;
    }

    if(m_Polygontype)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    ///
    glClear(GL_STENCIL_BUFFER_BIT);
    GLUtesselator* tess = tesser();
    if (!tess) return;
    glColor3d(0.0,1.0,1.0);//color setting
    gluTessBeginPolygon(tess,NULL);
    for(int i = 0; i < 8; i++)
    {
        gluTessVertex(tess, vertex_list[i], vertex_list[i]);
    }
    gluTessEndPolygon(tess);

    gluTessBeginPolygon(tess,NULL);
    glColor3d(1.0,1.0,1.0);//color setting
    for(int i = 15; i > 7; i--)
    {
        gluTessVertex(tess, vertex_list[i], vertex_list[i]);
    }
    gluTessEndPolygon(tess);
    gluDeleteTess(tess);
    ///
    GLubyte index[8][4]={
        0, 7, 15, 8,
        7, 6, 14, 15,
        6, 5, 13, 14,
        5, 4, 12, 13,
        4, 3, 11, 12,
        3, 2, 10, 11,
        2, 1, 9,  10,
        1, 0, 8,  9
    };

    for(int i = 0;i < 8; i++){
        glColor3dv(color_list[i]);//color setting
        glBegin(GL_POLYGON);
        for(int j =0; j<4; j++){
            glVertex3dv(vertex_list[index[i][j]]);
        }
        glEnd();
    }


    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    float R= 0.34f;
    glPushMatrix();
    glTranslatef(-1,1.68,0.0);
    glColor3f(1,1,1);
    //    glNormal3f(0,-1,0);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0;i<20;i++){
        glVertex3f(R*cos(2*M_PI/20*i),1,R*sin(2*M_PI/20*i));
    }
    glEnd();

    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    glColor3f(0.3,0.6,0.9);
    for(int i = 0;i<20;i++){
        glBegin(GL_POLYGON);
        glVertex3f(R*cos(2*M_PI/20*i),     1,       R*sin(2*M_PI/20*i));
        glVertex3f(R*cos(2*M_PI/20*i+1),   1,       R*sin(2*M_PI/20*i+1));
        glVertex3f(R*cos(2*M_PI/20*i+1),   -1.0/1.6,R*sin(2*M_PI/20*i+1));
        glVertex3f(R*cos(2*M_PI/20*i),     -1.0/1.6,R*sin(2*M_PI/20*i));
        glEnd();
    }

    glPopMatrix();

    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    glColor3f(1,1,1);
    glTranslatef(1,1.68,0.0);
    glColor3f(1,1,1);
    glNormal3f(0,1,0);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0;i<20;i++){
        glVertex3f(R*cos(2*M_PI/20*i),1,R*sin(2*M_PI/20*i));
    }
    glEnd();

    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }
    glColor3f(0.3,0.6,0.9);
    for(int i = 0;i<20;i++){
        glBegin(GL_POLYGON);
        glVertex3f(R*cos(2*M_PI/20*i),     1,       R*sin(2*M_PI/20*i));
        glVertex3f(R*cos(2*M_PI/20*i+1),   1,       R*sin(2*M_PI/20*i+1));
        glVertex3f(R*cos(2*M_PI/20*i+1),   -1.0/1.6,R*sin(2*M_PI/20*i+1));
        glVertex3f(R*cos(2*M_PI/20*i),     -1.0/1.6,R*sin(2*M_PI/20*i));
        glEnd();
    }
    glPopMatrix();



    //    for(int i = 8;i>0;i--){
    //        glVertex3d(i*0.125*0.5,2,sqrt((1-0.125*i)*(1-0.125*i))*-0.5);
    //    }
    //    for(int i = 0;i<8;i++){
    //        glVertex3d(i*0.125*-0.5,2,sqrt((1-0.125*i)*(1-0.125*i))*0.5);
    //    }
    //    for(int i = 8;i>0;i--){
    //        glVertex3d(i*0.125*-0.5,2,sqrt((1-0.125*i)*(1-0.125*i))*0.5);
    //    }


#if 0
    if(m_blank){
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        if(!m_Polygontype){
            glPolygonOffset(1.1f,4.0f);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glColorMask(1,1,1,0);
            glDisable(GL_TEXTURE_2D);
        }
    }
    else {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        if(!m_Polygontype){
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }

    ///cylinder
    /// left
    GLUquadricObj *quadObj1 = gluNewQuadric();
    glPushMatrix();
    glScalef(1,1.3,1.3);
    glTranslatef(-1,1.68,0.0);
    glRotatef(90,1.0,0.0,0.0);

    glColor3f(1.0,1.0,0.0);
    if(m_Polygontype)
        gluQuadricDrawStyle(quadObj1,GLU_FILL);
    else
        gluQuadricDrawStyle(quadObj1,GLU_LINE);
    gluQuadricNormals(quadObj1,GLU_SMOOTH);
    gluQuadricOrientation(quadObj1,GLU_INSIDE);
    gluQuadricTexture(quadObj1,GL_TRUE);

    gluCylinder(quadObj1,0.3,0.3,1.0,20.0,14.0);
    glPopMatrix();

    //
    GLUquadricObj *quadObj2;
    quadObj2 = gluNewQuadric();
    glPushMatrix();
    glScalef(1,1.3,1.3);
    glTranslatef(-1,1.68,0.0);
    glRotatef(90,1.0,0.0,0.0);

    glColor3f(1.0,1.0,1.0);
    if(m_Polygontype)
        gluQuadricDrawStyle(quadObj2,GLU_FILL);
    else
        gluQuadricDrawStyle(quadObj2,GLU_LINE);

    gluQuadricNormals(quadObj2,GLU_SMOOTH);
    gluQuadricOrientation(quadObj2,GLU_OUTSIDE);
    gluQuadricTexture(quadObj2,GL_TRUE);
    gluDisk(quadObj2,0.0,0.3,20.0,14.0);
    glPopMatrix();
    ///right
    GLUquadricObj *quadObj3 = gluNewQuadric();
    glPushMatrix();
    glScalef(1,1.3,1.3);
    glTranslatef(1,1.68,0.0);
    glRotatef(90,1.0,0.0,0.0);

    glColor3f(1.0,1.0,0.0);
    if(m_Polygontype)
        gluQuadricDrawStyle(quadObj3,GLU_FILL);
    else
        gluQuadricDrawStyle(quadObj3,GLU_LINE);
    gluQuadricNormals(quadObj3,GLU_SMOOTH);
    gluQuadricOrientation(quadObj3,GLU_INSIDE);
    gluQuadricTexture(quadObj3,GL_TRUE);


    gluCylinder(quadObj3,0.3,0.3,1.0,20.0,14.0);
    glPopMatrix();

    //
    GLUquadricObj *quadObj4;
    quadObj4 = gluNewQuadric();
    glPushMatrix();
    glScalef(1,1.3,1.3);
    glTranslatef(1,1.68,0.0);
    glRotatef(90,1.0,0.0,0.0);

    glColor3f(1.0,1.0,1.0);
    if(m_Polygontype)
        gluQuadricDrawStyle(quadObj4,GLU_FILL);
    else
        gluQuadricDrawStyle(quadObj4,GLU_LINE);

    gluQuadricNormals(quadObj4,GLU_SMOOTH);
    gluQuadricOrientation(quadObj4,GLU_OUTSIDE);
    gluQuadricTexture(quadObj4,GL_TRUE);
    gluDisk(quadObj4,0.0,0.3,20.0,14.0);
    glPopMatrix();

    if(m_blank){
        glColorMask(1,1,1,1);
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    gluDeleteQuadric(quadObj1);
    gluDeleteQuadric(quadObj2);
    gluDeleteQuadric(quadObj3);
    gluDeleteQuadric(quadObj4);
#endif
    glPopMatrix();
}  

void NeHeWidget::resizeGL(int width, int height)
{
    // Prevent window size from 0
    if ( height == 0 )
    {
        height = 1;
    }
    // Reset the current viewport
    glViewport( 0, 0, (GLint)width, (GLint)height );
    //Select projection matrix
    glMatrixMode( GL_PROJECTION );
    //reset projection matrix
    glLoadIdentity();
    //set viewport size
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    //Select observation model  matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}
#if 0
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,1.5);
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
glLightf(GL_LIGHT1, GL_SPOT_CUTOFF , 50.0);
glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);


///material light
glMaterialfv(GL_FRONT,GL_AMBIENT,color_ambient[i]);
glMaterialfv(GL_FRONT,GL_DIFFUSE,color_diffuse[i]);
glMaterialfv(GL_FRONT,GL_SPECULAR,color_specular[i]);
glMaterialf(GL_FRONT,GL_SHININESS,10);

static GLfloat color_ambient[8][3]={
    0.8f, 0.2f, 0.5f,
    0.5f, 0.8f, 0.8f,
    0.8f, 0.9f, 0.0f,
    0.6f, 0.3f, 0.7f,
    0.2f, 0.1f, 0.2f,
    0.0f, 0.4f, 0.3f,
    0.1f, 0.6f, 0.4f,
    0.3f, 0.7f, 0.1f,
};
static GLfloat color_diffuse[8][3]={
    0.8f, 0.2f, 0.0f,
    0.5f, 0.8f, 0.3f,
    0.8f, 0.9f, 0.5f,
    0.6f, 0.3f, 0.2f,
    0.2f, 0.1f, 0.7f,
    0.0f, 0.4f, 0.8f,
    0.1f, 0.6f, 0.9f,
    0.3f, 0.7f, 0.6f,
};
static GLfloat color_specular[8][3]={
    0.1f, 0.1f, 0.0f,
    0.8f, 0.7f, 0.3f,
    0.5f, 0.8f, 0.5f,
    0.7f, 0.2f, 0.2f,
    0.8f, 0.0f, 0.7f,
    0.1f, 0.3f, 0.8f,
    0.2f, 0.5f, 0.9f,
    0.9f, 0.2f, 0.6f,
};
#endif
