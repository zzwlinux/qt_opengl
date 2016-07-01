#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include<QPainterPath>

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QPoint Point;   /* 线条绘制点的坐标 */

    void startPainting(void);    /* 开启绘画曲线 也就是开启定时器 */
    void closePainting(void);    /* 停止绘画曲线 也就是停止定时器 */


    void paintEvent(QPaintEvent *); /* 重绘事件函数 */

private slots:
    void timerUpDate();     /* 定时器槽函数 */

private:
    unsigned int t; /* 用来保存时间 */
    int p; /* 用来调整坐标原点 */

    QPainterPath *path_src,*path_kalman;
    QTimer *timer;
};

#endif // DIALOG_H
