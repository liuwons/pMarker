#ifndef LIMAGE_H
#define LIMAGE_H

#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"

#include <QImage>
#include <QRect>
#include <QPoint>

#include "lmath.h"


/**
  *sImage 为 resize 之后的图像
  */
class LImage
{
private:

public:
    IplImage* image;
    IplImage* sImage;
    double    resizeScale;

    LImage();
    LImage(const char* imgName);
    LImage(IplImage* img);
    ~LImage();
    void setImage(IplImage* img);
    int resize(double scale);
    int resize(int width, int height);
    QImage* resizedQImage();
    static void rgbSwap(IplImage* img);

    QRect toOriginalRect(QRect rect);
    QPoint toOriginalPos(QPoint p);
};

#endif // LIMAGE_H
