#ifndef PREVIEW_H
#define PREVIEW_H

#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"

#include <QWidget>
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QPalette>
#include <QImage>
#include <QDebug>
#include <QMouseEvent>
#include <QResizeEvent>

#include "limage.h"

class Preview : public QWidget
{
    Q_OBJECT

private:


    void init();

public:
    const static int PREVIEW_OVERALL = 0;
    const static int PREVIEW_PART    = 1;

    QRect* showRect;   //rImage��Ҫ��ʾ�ľ�������
    QRect* clipRect;   //�ֲ�Ԥ�����õ��ı����
    LImage* lImage;
    int type;
    bool quickMove;

    QRect* drawRect;   //Preview��������ʾ������

    explicit Preview(QWidget *parent = 0);
    Preview(QSize size, int t, QWidget *parent = 0);
    ~Preview();
    void setQuickMove(bool b);

    void updateShowInfo();
    
signals:
    void quickMoveEvent(int x, int y);
    
public slots:
    void setImage(IplImage* img);
    void setShowRect(QRect rect = QRect(0, 0, 0, 0));

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    
};

#endif // PREVIEW_H
