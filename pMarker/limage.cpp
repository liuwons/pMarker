#include "limage.h"
#include <math.h>
#include <QDebug>

/**
  *默认构造函数
  */
LImage::LImage()
{
    image  = 0;
    sImage = 0;
    resizeScale = 0;
}

/**
  *构造函数，imgName为需要加载的图像的文件名，由于Qt的图像通道为RGB，而opencv的图像通道为BGR，因此需要进行通道转换
  *qImage图像格式目前为RGB-888
  */
LImage::LImage(const char* imgName)
{
    image  = 0;
    sImage = 0;
    resizeScale = 0;
    image = cvLoadImage(imgName);
    if(image != 0 && image->depth == 8 && image->nChannels == 3)
    {
        rgbSwap(image);
    }
    resize(1);
    resizeScale = 1;
}

LImage::LImage(IplImage* img)
{
    image = cvCloneImage(img);
    sImage = 0;
    resizeScale = 0;
}

void LImage::setImage(IplImage* img)
{
    image = img;
    sImage = 0;
}

/**
  *析构函数
  */
LImage::~LImage()
{
    if(image != 0)
        cvReleaseImage(&image);
    if(sImage != 0)
        cvReleaseImage(&sImage);
}

/**
  *缩放原图像
  *@param scale 缩放尺寸
  */
int LImage::resize(double scale)
{
    if(image == 0)
        return -1;
    if(resizeScale == scale)
    {
        return 0;
    }
    if(sImage != 0)
        cvReleaseImage(&sImage);

    int sw, sh, dw, dh;
    sw = image->width;
    sh = image->height;
    dw = (int)floor(sw * scale);
    dh = (int)floor(sh * scale);
    sImage = cvCreateImage(cvSize(dw, dh), image->depth, image->nChannels);
    cvResize(image,sImage,CV_INTER_NN);
    resizeScale = scale;

    return 0;
}

int LImage::resize(int width, int height)
{
    if(image == 0)
        return -1;
    if(image->width == 0 || image->height == 0)
        return -2;
    double scale, sx, sy;
    sx = (double)width / image->width;
    sy = (double)height / image->height;
    scale = sx < sy ? sx : sy;
    return resize(scale);
}

/**
  *对深度8位、3个频道的图像进行rgb转置
  */
void LImage::rgbSwap(IplImage* img)
{
    uchar tmp;
    int i, j;
    uchar *p = (uchar*)img->imageData;
    for(i = 0; i < img->height; i ++)
    {
        for(j = 0; j < img->width; j ++)
        {
            tmp = p[j * 3];
            p[j * 3] = p [j * 3 + 2];
            p[j * 3 + 2] = tmp;
        }
        p += img->widthStep;
    }
}

/**
  *所得的qImage与zImage公用图像数据缓冲区，并且在消亡时是不会消除图像数据缓冲区的
  */
QImage* LImage::resizedQImage()
{
    QImage* qImage = 0;
    if(sImage != 0 &&sImage->depth == 8 && sImage->nChannels == 3)
    {
        qImage = new QImage((uchar*)sImage->imageData, sImage->width, sImage->height, sImage->widthStep, QImage::Format_RGB888);
    }
    return qImage;
}

QRect LImage::toOriginalRect(QRect rect)
{
    //int x = (int)((double)rect.x() / resizeScale);
    //int y = (int)((double)rect.y() / resizeScale);
    //int w = (int)((double)rect.width() / resizeScale);
    //int h = (int)((double)rect.height() / resizeScale);
    int x = lRound((double)rect.x() / resizeScale);
    int y = lRound((double)rect.y() / resizeScale);
    int w = lRound((double)rect.width() / resizeScale);
    int h = lRound((double)rect.height() / resizeScale);
    if((rect.x() + rect.width()) == sImage->width)    //不能框全图bug修复,1018
        w = image->width - x;
    if((rect.y() + rect.height()) == sImage->height)
        h = image->height - y;
    return QRect(x, y, w, h);
}

QPoint LImage::toOriginalPos(QPoint p)
{
    int x = (int)((double)p.x() / resizeScale);
    int y = (int)((double)p.y() / resizeScale);
    return QPoint(x, y);
}


