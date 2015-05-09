#ifndef PICFORM_H
#define PICFORM_H

#include <QWidget>
#include <QRect>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QLine>
#include <QPoint>
#include <QDebug>
#include <QCursor>
#include <QSize>
#include <QPalette>
#include <QVector>

#include "limage.h"
#include "values.h"
#include "lmath.h"
#include "lmark.h"

class PicForm : public QWidget
{
    Q_OBJECT

private:
    bool newMark;

public:
    LImage* lImage;     //当前要处理的图像结构
    QPoint showCenter;  //要显示的resizedImage的区域的中心点,image坐标系
    QRect* markRect;    //上次标记区域，picForm坐标系
    QRect markRectOnOriginal;  //标记区域，image坐标系
    QVector<MarkInfo> markedInfos; //已经标注的区域

    QPoint startPos;    //标注起始点，picForm坐标系
    QPoint endPos;      //当前鼠标点，picForm坐标系

    QRect showRect;      //显示图像的区域，picForm坐标系
    QRect imgRect;       //显示的图像的一部分的位置,resizedImage坐标系
    void calcRect();     //计算showRect和imgRect
    QRect toResizeRect(QRect r);     //从picForm坐标系转到lImage的resizedImage坐标系
    QPoint toResizePoint(QPoint p);  //从picForm坐标系转到lImage的resizedImage坐标系
    QRect originalToPicRect(QRect rect);   //从image坐标系到picForm坐标系
    QPoint originalToPicPoint(QPoint p);
    int setCenter(QPoint p);                //设置显示中心点（以image为参照坐标）

    explicit PicForm(QRect* rect, QWidget* parent = 0);
    ~PicForm();
    QSize loadImage(const char* fileName, const LMark* lmark = 0);  //从文件加载图像
    void deleteRects(int x, int y);

signals:
    void cursorChanged(int x, int y);       //鼠标指向的点相对于原图像的位置
    void imageMarked(int x, int y, int width, int height);   //图像被标记，参数都是相对于原图像的
    void imageMarked(QRect r);              //图像被标记，image坐标系
    void changeImage(IplImage* img);
    void changeScale(double scale, int dir);
    void changeScale(double scale);
    void markAdded();
    void clearMarks();
    void writeToData();

public slots:
    void resizeImage(double scale);
    void resizeImage(int w, int h, double* scale_buf = 0); //scale_buf为要填入resizeScale的缓冲区,默认为空
    int setCenter(int x, int y);
    void addMark();           //标记后按下相关Key添加标记
    void clearAllMark();

protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // PICFORM_H
