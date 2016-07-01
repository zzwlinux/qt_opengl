#include "mymainwindow.h"
#include <QTextCodec>
#include <QUrl>
#include <QSlider>

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),Waveform(NULL),neheWidget(NULL)
{

    neheWidget = new NeHeWidget(this);

    this->setFixedSize(1200,720);
    setWindowTitle("OpenGL--张臻炜");
    fullscreen = false;

    centralWidget = new QWidget(this);
    centralWidget->setWindowIcon(QIcon(":/image/data/mainwin.png"));

    mdiArea = new QMdiArea(centralWidget);
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(3, 3, 3, 3);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    //centralWidget ->setGeometry();
    //QTextEdit::setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(mdiArea, 0, 0, 1, 1);
    setCentralWidget(neheWidget);

    dock = new QDockWidget();
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    dock->setMinimumWidth(200);

    QWidget *dockwidget = new QWidget;
    creatAction();

    initSpeedGauge();
    QVBoxLayout *dockLayout = new QVBoxLayout(dockwidget);
    dockLayout->addWidget(mSpeedGauge);

    QSlider *speedCount = new QSlider(Qt::Horizontal);
    speedCount->setMinimum(0);
    speedCount->setMaximum(80);
    speedCount->setValue(0);
    dockLayout->addWidget(speedCount);
    dockLayout->addStretch(5);
    connect(speedCount,SIGNAL(valueChanged(int)),neheWidget,SLOT(setSpeed(int)));
    connect(neheWidget,SIGNAL(speedUpdate(int)),speedCount,SLOT(setValue(int)));

    dock->setWidget(dockwidget);
    this->addDockWidget(Qt::RightDockWidgetArea,dock,Qt::Vertical);

    connect(neheWidget,SIGNAL(speedUpdate(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    initMenu();
    initToolBar();
    initStateBar();
}


void MyMainWindow::creatAction()
{
    actionList = new QList<QAction*>;

    filecloseAction = new QAction("&Close...",this);
    filecloseAction->setShortcut(QKeySequence::fromString(tr("ctrl+q")));
    connect(filecloseAction, SIGNAL(triggered()), this, SLOT(action_Quit()));

    actionList->append(filecloseAction);
    // add all actions to main window
    foreach(QAction* action, *actionList) {
        //add to MainWindow so they work when menu is hidden
        this->addAction(action);
    }
}

void MyMainWindow::initMenu()
{
    /* initial File menu */
    fileMenu = new QMenu(tr("File"), this);

    fileOpenAction = new QAction("&Open...", this);
    fileSaveAction = new QAction("&Save...", this);

    //filecloseAction->setStatusTip(tr("Quit the application"));

    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(filecloseAction);

    /* initial Edit */
    editMenu = new QMenu("&Edit");
    editCopyAction = editMenu->addAction("&Copy");
    editCutAction = editMenu->addAction("&Cut");

    /* initial View */
    ///
    viewMenu = new QMenu("&View");
    QMenu *statebarMenu = viewMenu->addMenu("&StateBar");
    QAction *showstate = statebarMenu->addAction("&Show/Hide");
    connect(showstate, SIGNAL(triggered()), this, SLOT(action_ShowHideStatusBar()));

    ///
    QMenu *winsizeMenu = viewMenu->addMenu("&ShowFullScreen");
    QAction *FullScreen = winsizeMenu->addAction("&FullScreen?");
    connect(FullScreen, SIGNAL(triggered()), this, SLOT(fullScreen()));

    ///
    QMenu *dockMenu = viewMenu->addMenu("&Dockwidget");
    QAction *ShowDock = dockMenu->addAction("&ShowDock?");
    connect(ShowDock, SIGNAL(triggered()), this, SLOT(showdock()));
    QAction *FullScreenDock = dockMenu->addAction("&FullScreenDock?");
    connect(FullScreenDock, SIGNAL(triggered()), this, SLOT(fullScreenDock()));

    ///
    QMenu *WaveformMenu = viewMenu->addMenu("&Waveform");
    QAction *showWaveform = WaveformMenu->addAction("&Show/Hide");
    connect(showWaveform, SIGNAL(triggered()), this, SLOT(action_ShowHideWaveform()));

    QAction *stopWaveform = WaveformMenu->addAction("&restart/stop");
    connect(stopWaveform, SIGNAL(triggered()), this, SLOT(action_deleteWaveform()));

    ///
    QMenu *GLMenu = viewMenu->addMenu("&OpenGL");
    QAction *showGL = GLMenu->addAction("&Show/Hide");
    connect(showGL, SIGNAL(triggered()), neheWidget, SLOT(hide()));

    /* initial speed */
    speedMenu = new QMenu(tr("速度"), this);

    group = new QActionGroup(this);

    actionNew=new QAction(tr("减速"),this);
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setlowspeed()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    speedMenu->addAction(actionNew);

    actionNew=new QAction(tr("中速"),this);
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setMidlespeed()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    speedMenu->addAction(actionNew);

    actionNew=new QAction(tr("加速"),this);
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setFastspeed()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    speedMenu->addAction(actionNew);

    /* add menu to menubar */
    QMenuBar *menuBar = this->menuBar();
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(speedMenu);
    menuBar->addMenu(viewMenu);
}

void MyMainWindow::initMouseMenu()
{
    /// display menu
    QMenu *displayMenu_ = new QMenu(tr("显示"), this);
    QActionGroup *group = new QActionGroup(this);

    QAction *actionNew=new QAction(tr("线框图"),this);
    connect(actionNew, SIGNAL(triggered()),neheWidget,SLOT(SetPolyMode_line()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    displayMenu_->addAction(actionNew);

    actionNew=new QAction(tr("填充图"),this);
    connect(actionNew, SIGNAL(triggered()),neheWidget,SLOT(SetPolyMode_fill()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    displayMenu_->addAction(actionNew);

    mouseMenu->addMenu(displayMenu_);

    /// blanking menu
    QMenu *blankingMenu_ = new QMenu(tr("消隐"), this);
    group = new QActionGroup(this);

    actionNew=new QAction(tr("消隐"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(SetBlank()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    blankingMenu_->addAction(actionNew);

    actionNew=new QAction(tr("不消隐"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(cancelBlank()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    blankingMenu_->addAction(actionNew);

    mouseMenu->addMenu(blankingMenu_);

    /// Lighting  menu
    QMenu *LightingMenu_ = new QMenu(tr("光照"), this);
    group = new QActionGroup(this);

    actionNew=new QAction(tr("环境光"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setAMBIENT_Light()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    LightingMenu_->addAction(actionNew);

    actionNew=new QAction(tr("漫反射"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setDIFFUSE_Light()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    LightingMenu_->addAction(actionNew);

    actionNew=new QAction(tr("镜面反射"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setSPECULAR_Light()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    LightingMenu_->addAction(actionNew);

    actionNew=new QAction(tr("全部反射"),this);
    connect(actionNew, SIGNAL(triggered()), neheWidget, SLOT(setGloable_Light()));
    group->addAction(actionNew);
    actionNew->setCheckable(true);
    LightingMenu_->addAction(actionNew);

    mouseMenu->addMenu(LightingMenu_);
}

void MyMainWindow::rotationX()
{

}

void MyMainWindow::rotationY()
{

}

void MyMainWindow::initToolBar()
{
    /* Init FileToolBar */
    fileToolBar = new QToolBar(this);
    fileToolBar->addAction(fileOpenAction);
    fileToolBar->addAction(fileSaveAction);
    /* Init EditToolBar */
    editToolBar = new QToolBar(this);
    editToolBar->addAction(editCopyAction);
    editToolBar->addAction(editCutAction);
    /*QToolButton */
    toolBtn = new QToolButton(this);
    toolBtn->setText(tr("Close Dock"));

    // add a new menu
    group1 = new QActionGroup(this);
    QMenu *closeMenu = new QMenu(this);

    /* open dockwidget */
    actionNew = closeMenu->addAction(tr("open"));
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    actionNew->setIcon(QIcon(":/image/data/show.png"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(opendock()));
    group1->addAction(actionNew);
    actionNew->setCheckable(true);

    /* close dockwidget */
    actionNew =  closeMenu->addAction(tr("off"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(closedock()));
    actionNew->setIcon(QIcon(":/image/data/close.png"));
    group1->addAction(actionNew);
    actionNew->setCheckable(true);

    // 添加菜单
    toolBtn->setMenu(closeMenu);

    // 设置弹出模式
    toolBtn->setPopupMode(QToolButton::MenuButtonPopup);

    // 向工具栏添加QToolButton按钮

    editToolBar->addWidget(toolBtn);
    /* 将工具添加到工具栏上 */
    addToolBar(Qt::TopToolBarArea, fileToolBar);
    addToolBar(Qt::TopToolBarArea, editToolBar);

}

// show temporary message，which last for 2000 ms
void MyMainWindow::initStateBar(){
    statusBar = new QStatusBar(this);
    statusBar->showMessage(tr("西北工业大学 (Northwestern Polytechnical University)"),2000);

//    QProgressBar *my_progressbar = new QProgressBar(this);
    // creat label
    my_lable *permanent = new my_lable(this);

    // 标签样式
    permanent->setFrameStyle(QFrame::Box | QFrame::Sunken);

    // 显示信息
    permanent->setText("https://github.com/zzwlinux/qt_opengl.git");
    connect(permanent,SIGNAL(openurl(QString)),this,SLOT(openweb(QString)));
//    statusBar->addPermanentWidget(my_progressbar);
//    my_progressbar->setTextVisible( true );
//    my_progressbar->setValue(30);
    // 将标签设置为永久部件
    statusBar->addPermanentWidget(permanent);
    this->setStatusBar(statusBar);
}

// 新建文件菜单
void MyMainWindow::on_action_New_triggered()
{
    // 新建文本编辑器部件
    QTextEdit *edit = new QTextEdit(this);

    // 使用QMdiArea类的addSubWindow()函数创建子窗口，以文本编辑器为中心部件
    QMdiSubWindow *child = mdiArea->addSubWindow(edit);

    child->setWindowTitle(tr("多文档编辑器子窗口"));

    // 显示子窗口
    child->show();
}

MyMainWindow::~MyMainWindow()
{

}

/* slot */
void MyMainWindow::lowspeed()
{

}

void MyMainWindow::midspeed()
{

}

void MyMainWindow::highspeed()
{

}

void MyMainWindow::closedock()
{
    if (!dock->isHidden())
        dock->hide();
}

void MyMainWindow::opendock()
{
    if (dock->isHidden())
        dock->show();
}

void MyMainWindow::showdock()
{
    if (dock->isHidden())
        dock->show();
    else {
        dock->hide();
    }
}


void MyMainWindow::action_ShowHideStatusBar(void)
{
    if( statusBar->isHidden() )
        statusBar->setHidden(false);
    else
        statusBar->setHidden(true);
}

void MyMainWindow::action_ShowHideWaveform(void)
{
    if( Waveform == NULL )
        Waveform = new Dialog(centralWidget);
    if(Waveform->isHidden())
        Waveform->show();
    else {
        Waveform->hide();
    }
}

void MyMainWindow::action_ShowHideGL(void)
{
    if( neheWidget == NULL )
        neheWidget = new NeHeWidget(this);
    if(neheWidget->isHidden())
        neheWidget->show();
    else {
        neheWidget->hide();
    }
}

void MyMainWindow::action_deleteWaveform(void)
{
    if( Waveform != NULL )
    {
        delete Waveform;
        Waveform = NULL;
    }
    else {
        action_ShowHideWaveform();
    }
}


void MyMainWindow::fullScreen(void)
{
    if( this->isFullScreen() )
        this->showNormal();
    else
        this->showFullScreen();
}



void MyMainWindow::fullScreenDock(void)
{
    dock->setWindowFlags(Qt::Window);
    if( dock->isFullScreen() )
        dock->showNormal();
    else
        dock->showFullScreen();
}



void MyMainWindow::action_Quit(void)
{
    QMessageBox msgbox;
    msgbox.setWindowTitle("Confirm");
    msgbox.setText("Are you sure to quit?");
    msgbox.setStandardButtons( QMessageBox::Cancel | QMessageBox::Ok );
    msgbox.setDefaultButton(QMessageBox::Cancel);
    msgbox.show();
    msgbox.move(geometry().center().x()-msgbox.size().width()/2,
                geometry().center().y()-msgbox.size().height()/2);
    // msgbox.exec();

    //forbidden any other Application
    //禁止其他屏幕操作
    msgbox.setWindowModality(Qt::ApplicationModal);

    //防止点击cancel还退出
    QApplication::setQuitOnLastWindowClosed(false);
    if( msgbox.exec() == QMessageBox::Ok )
        qApp->exit(0);
}

void MyMainWindow::openweb(QString http)
{
    QUrl url(http);
    QDesktopServices::openUrl(url);
}

void MyMainWindow::on_horizontalSlider_valueChanged(int value)
{
    mSpeedNeedle->setCurrentValue(value);
}

void MyMainWindow::overspeedwarning()
{
    QcBackgroundItem *bkg1 = mSpeedGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::yellow);
    bkg1->addColor(1.0,Qt::red);
}

bool MyMainWindow::screenshot(bool full_)
{
    //        QDesktopWidget * pDesktoWidget = QApplication::desktop();
    QScreen * pqscreen  = QGuiApplication::primaryScreen() ;
    QPixmap pixmap ;
    if(full_)
        pixmap = pqscreen->grabWindow(QApplication::desktop()->winId());
//                                      this->x(),
//                                      this->y(),
//                                      this->width(),
//                                      this->height() );
    else if(neheWidget!= NULL)
        pixmap = pqscreen->grabWindow(0);
//                                       neheWidget->x(),
//                                       neheWidget->y(),
//                                       neheWidget->width(),neheWidget->height() );

    QDateTime ctime;
    ctime = QDateTime::currentDateTime();
    QString ss = ctime.toString("yyyy-MM-dd_hh-mm-ss");
    ss += ".jpg";
    bool ret = pixmap.save(ss, "jpg");
    return ret;
}

void MyMainWindow::initSpeedGauge()
{
    mSpeedGauge = new QcGaugeWidget(dock);
    mSpeedGauge->addBackground(99);
    QcBackgroundItem *bkg1 = mSpeedGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);


    QcBackgroundItem *bkg2 = mSpeedGauge->addBackground(88);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    mSpeedGauge->addArc(55);
    mSpeedGauge->addDegrees(65)->setValueRange(0,80);
    mSpeedGauge->addColorBand(50);

    mSpeedGauge->addValues(80)->setValueRange(0,80);

    mSpeedGauge->addLabel(70)->setText("Km/h");
    QcLabelItem *lab = mSpeedGauge->addLabel(40);
    lab->setText("0");
    mSpeedNeedle = mSpeedGauge->addNeedle(60);
    mSpeedNeedle->setLabel(lab);
    mSpeedNeedle->setColor(Qt::white);
    mSpeedNeedle->setValueRange(0,80);
    mSpeedGauge->addBackground(7);
    mSpeedGauge->addGlass(88);
    mSpeedGauge->show();
}


void MyMainWindow::keyPressEvent(QKeyEvent *e)
{
    switch ( e->key() )
    {
    case Qt::Key_R:
        neheWidget->setRotationDirect(rotaiotnXY::X);
        neheWidget->start_thread();
        break;

    case Qt::Key_F1:
        neheWidget->setRotationDirect(rotaiotnXY::Y);
        neheWidget->start_thread();
        break;

    case Qt::Key_F3:
        neheWidget->setRotationDirect(rotaiotnXY::Z);
        neheWidget->start_thread();
        break;

    case Qt::Key_F2:
        fullscreen = !fullscreen;
        if ( fullscreen )
        {
            showFullScreen();
        }
        else
        {
            showNormal();
        }
        neheWidget->updateGL();
        break;

    case Qt::Key_S:
        if(e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) )
            screenshot(true);
        else
            screenshot(false);
        break;
    case Qt::Key_Escape:
        close();
        break;
    }
}

void MyMainWindow::mousePressEvent(QMouseEvent *event)
{
    switch ( event->button() )
    {
//    case Qt::LeftButton:
//        fullscreen = !fullscreen;
//        if ( fullscreen )
//        {
//            showFullScreen();
//        }
//        else
//        {
//            showNormal();
//        }
//        break;
    case Qt::RightButton:
        mouseMenu =  new QMenu(this);

        initMouseMenu();
        mouseMenu->exec(QCursor::pos());
        mouseMenu->show();
        break;
    }
}

