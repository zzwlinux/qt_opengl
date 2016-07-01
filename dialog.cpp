#include "dialog.h"
#include <iostream>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),timer(NULL),path_kalman(NULL),path_src(NULL)
{
    this->setMinimumSize(1000,500);
    this->setWindowTitle("Waveform");
    p = t = 0;

    Point.setX(0);
    Point.setY(0);

    path_src    = new QPainterPath;
    path_kalman = new QPainterPath;
    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(200);
}

Dialog::~Dialog()
{
    if(path_src!=NULL)
        delete path_src;
    if(path_kalman!=NULL)
        delete path_kalman;
    if(timer!=NULL)
        delete timer;
}


void Dialog::timerUpDate()
{
    t += 10;
    Point.setX(t);
    Point.setY(qrand() % int(this->height()*0.4)+50);
    path_src->lineTo(Point);
    path_kalman->lineTo(Point.x(),Point.y()+qrand() % 20);

    if(t > width())
        p -= 10;

    update();
}

void Dialog::startPainting(void)
{
    timer->start(100);
}

void Dialog::closePainting(void)
{
    timer->stop();
}

void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3 ));

    painter.drawLine(QPoint(0,0),QPoint(0,this->width()));
    painter.drawLine(QPoint(0,this->height()),QPoint(this->width(),this->height()));

    painter.setPen(QPen(Qt::red, 2));

    painter.translate(p,0);
    painter.scale(1.0,1);

    painter.drawPath(*path_src);

    painter.setPen(QPen(Qt::blue, 2));

    painter.drawPath(*path_kalman);
}
