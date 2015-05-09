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
    LImage* lImage;     //��ǰҪ�����ͼ��ṹ
    QPoint showCenter;  //Ҫ��ʾ��resizedImage����������ĵ�,image����ϵ
    QRect* markRect;    //�ϴα������picForm����ϵ
    QRect markRectOnOriginal;  //�������image����ϵ
    QVector<MarkInfo> markedInfos; //�Ѿ���ע������

    QPoint startPos;    //��ע��ʼ�㣬picForm����ϵ
    QPoint endPos;      //��ǰ���㣬picForm����ϵ

    QRect showRect;      //��ʾͼ�������picForm����ϵ
    QRect imgRect;       //��ʾ��ͼ���һ���ֵ�λ��,resizedImage����ϵ
    void calcRect();     //����showRect��imgRect
    QRect toResizeRect(QRect r);     //��picForm����ϵת��lImage��resizedImage����ϵ
    QPoint toResizePoint(QPoint p);  //��picForm����ϵת��lImage��resizedImage����ϵ
    QRect originalToPicRect(QRect rect);   //��image����ϵ��picForm����ϵ
    QPoint originalToPicPoint(QPoint p);
    int setCenter(QPoint p);                //������ʾ���ĵ㣨��imageΪ�������꣩

    explicit PicForm(QRect* rect, QWidget* parent = 0);
    ~PicForm();
    QSize loadImage(const char* fileName, const LMark* lmark = 0);  //���ļ�����ͼ��
    void deleteRects(int x, int y);

signals:
    void cursorChanged(int x, int y);       //���ָ��ĵ������ԭͼ���λ��
    void imageMarked(int x, int y, int width, int height);   //ͼ�񱻱�ǣ��������������ԭͼ���
    void imageMarked(QRect r);              //ͼ�񱻱�ǣ�image����ϵ
    void changeImage(IplImage* img);
    void changeScale(double scale, int dir);
    void changeScale(double scale);
    void markAdded();
    void clearMarks();
    void writeToData();

public slots:
    void resizeImage(double scale);
    void resizeImage(int w, int h, double* scale_buf = 0); //scale_bufΪҪ����resizeScale�Ļ�����,Ĭ��Ϊ��
    int setCenter(int x, int y);
    void addMark();           //��Ǻ������Key��ӱ��
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
