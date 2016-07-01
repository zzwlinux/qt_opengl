#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTextCodec>
#include <QActionGroup>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QMdiSubWindow>
#include <qmdisubwindow.h>
#include <QMdiArea>
#include <QMessageBox>
#include <QApplication>
#include <QProgressBar>
#include <QString>
#include "dialog.h"
#include "nehewidget.h"
#include "qcgaugewidget.h"


class my_lable : public QLabel
{
    Q_OBJECT
public:
    explicit my_lable(QWidget *parent = 0):QLabel(parent){}
    virtual void mousePressEvent(QMouseEvent *ev){
        if(ev->button()==Qt::LeftButton){
            emit openurl(this->text());
        }
    }
signals:
    void openurl(QString);

public slots:

};
class MyMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

signals:

public slots:
    void lowspeed();
    void midspeed();
    void highspeed();
    void opendock();
    void closedock();
    void on_action_New_triggered();
    void action_ShowHideStatusBar(void);
    void action_ShowHideWaveform(void);
    void action_ShowHideGL(void);
    void action_deleteWaveform(void);
    void fullScreen();
    void showdock();
    void fullScreenDock();
    void action_Quit(void);
    void openweb(QString http);
    void on_horizontalSlider_valueChanged(int value);
    void overspeedwarning(void);

private:
    bool screenshot(bool full_);
    void initSpeedGauge(void);

protected:
    bool fullscreen;
    void keyPressEvent( QKeyEvent *e );
    void mousePressEvent( QMouseEvent *event);
    void initMenu();
    void initMouseMenu();

    void rotationX();
    void rotationY();

    void initToolBar();
    void initStateBar();
    void creatAction();

    QList<QAction*>     *actionList;

    QGridLayout         *gridLayout;
    QMdiArea            *mdiArea;
    QStatusBar          *statusBar;
    QToolBar            *fileToolBar ,*editToolBar;
    QToolButton         *toolBtn;
    QMenu               *mouseMenu,*fileMenu,*editMenu,*speedMenu,*viewMenu;
    QTextEdit           *text;
    QWidget             *centralWidget;
    QDockWidget         *dock;
    QAction             *actionNew,*fileSaveAction,*fileOpenAction,
                        *filecloseAction,*editCutAction, *editCopyAction;
    QActionGroup        *group,*group1;


private:
    NeHeWidget           *neheWidget ;
    Dialog               *Waveform;
    QcGaugeWidget        *mSpeedGauge;
    QcNeedleItem         *mSpeedNeedle;
};

#endif // MYMAINWINDOW_H
