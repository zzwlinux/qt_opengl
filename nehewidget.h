#ifndef NEHEWIDGET_H
#define NEHEWIDGET_H  
#include <QGLWidget>  
#include <QtGui>  
#include <QThread>
#include <QtOpenGL>  
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

enum rotaiotnXY{ X, Y, Z };

class GLthread: public QThread
{
    Q_OBJECT
public:
    explicit GLthread(QObject *parent = 0)
    {}
    ~GLthread(){
        stop();
    }
    void stop(){
        this->exit();
    }
private:
    void run(){
        while(1){
            emit refresh_GL();
            Sleep(30);
        }
    }

signals:
    void refresh_GL(void);
public slots:
};

class NeHeWidget : public QGLWidget
{  
    Q_OBJECT
public:  
    explicit NeHeWidget(QWidget *parent = 0);
    ~NeHeWidget();
    void start_thread(void){
        if(!m_thread.isRunning())
            m_thread.start();
    }

    void setRotationDirect(rotaiotnXY dir = X)
    {
        m_direction = dir;
    }

signals:
    void update_(void);
    void speedUpdate(int);
private slots:
    void rotation(void);
    ///set polygon model
    void SetPolyMode_fill(void)
    {
        m_Polygontype = true;
        updateGL();
    }
    void SetPolyMode_line(void)
    {
        m_Polygontype = false;
        updateGL();
    }
    ///set blank
    void SetBlank(void)
    {
        m_blank = true;
        updateGL();
    }
    void cancelBlank(void)
    {
        m_blank = false;
        updateGL();
    }
    ///lighting
    void setGloable_Light(void)
    {
        //        GLfloat ambient[] = {1, 1, 1, 1.0};
        glDisable(GL_LIGHTING);
        //        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
        //        glEnable(GL_LIGHTING);
    }
    void setAMBIENT_Light(void)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
    }
    void setDIFFUSE_Light(void)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT3);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHTING);
    }
    void setSPECULAR_Light(void)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHTING);
    }
    ///speed setting
    void setFastspeed(){
        if(speed<20)
            speed += 1.0f;
    }
    void setMidlespeed(){
        speed = 10;
    }
    void setlowspeed(){
        if(speed>2)
            speed -= 1.0f;
    }

    void setSpeed(int speed_){
        if(speed_>=0 && speed_< 80)
        speed = speed_ /4 ;
    }
    void spinDisplay(void);

protected:
    virtual void run();

    //Set render environment
    void initializeGL();
    //paint window
    void paintGL();
    //Response to window size change
    void resizeGL( int width, int height );

protected:
    GLfloat         spin, speed; ///< angle and speed for rotation
    GLthread        m_thread;
    rotaiotnXY      m_direction;
    bool            m_Polygontype;
    bool            m_blank;
};  
#endif // NEHEWIDGET_H  
