#ifndef MAINWINDOW_H  
#define MAINWINDOW_H  
#include <QMainWindow>
#include <QKeyEvent>  
#include "nehewidget.h"  

class MainWindow : public QMainWindow  
{  
    Q_OBJECT  
public:  
    MainWindow(QWidget *parent = 0);  
    ~MainWindow();  
protected:  
    bool fullscreen;  
    void keyPressEvent( QKeyEvent *e );
    void mousePressEvent(QMouseEvent *event);
private:  
    NeHeWidget *neheWidget ;
};  
#endif // MAINWINDOW_H 
