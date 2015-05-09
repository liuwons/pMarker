#include "preview.h"

Preview::Preview(QWidget *parent) :
    QWidget(parent)
{
    init();
}

Preview::Preview(QSize size, int t, QWidget *parent) : QWidget(parent)
{
    type = t;
    this->setGeometry(QRect(0, 0, size.width(), size.height()));
    init();
}

Preview::~Preview()
{
    if(showRect != 0)
        delete showRect;
    if(clipRect != 0)
        delete clipRect;
    if(lImage != 0)
        delete lImage;
    if(drawRect != 0)
        delete drawRect;
}

void Preview::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if(lImage == 0 || *showRect == QRect(0, 0, 0, 0) || *drawRect == QRect(0, 0, 0, 0))
        return;
    QPainter p(this);
    QImage* img = lImage->resizedQImage();
    p.drawImage(*drawRect, *img, *showRect);
    delete img;
}

void Preview::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void Preview::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);

    if(e->button() == Qt::LeftButton)
    {
        if((type == PREVIEW_PART) || (!quickMove))
            return;
        int ofx = e->x() - drawRect->x();
        int ofy = e->y() - drawRect->y();
        int x = (int)((double)ofx / lImage->sImage->width * lImage->image->width);
        int y = (int)((double)ofy / lImage->sImage->height * lImage->image->height);
        emit quickMoveEvent(x, y);
    }
}

void Preview::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void Preview::setImage(IplImage* img)
{
    if(lImage != 0)
        delete lImage;
    lImage = new LImage(img);
    if(type == PREVIEW_OVERALL)
    {
    }
    else if(type == PREVIEW_PART)
    {
        *showRect = QRect(0, 0, 0, 0);
        *drawRect = QRect(0, 0, 0, 0);
        clipRect->setRect(0, 0, 0, 0);
    }
    updateShowInfo();
    update();
}

//只应用于局部预览
void Preview::setShowRect(QRect rect)
{
    clipRect->setRect(rect.x(), rect.y(), rect.width(), rect.height());
    updateShowInfo();
    update();
}

void Preview::init()
{
    quickMove = false;
    lImage = 0;
    showRect = new QRect(0, 0, 0, 0);
    drawRect = new QRect(0, 0, 0, 0);
    clipRect = new QRect(0, 0, 0, 0);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::gray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void Preview::updateShowInfo()
{
    if(type == PREVIEW_OVERALL)
    {
        lImage->resize(this->width(), this->height());
        int x = (this->width() - lImage->sImage->width) / 2;
        int y = (this->height() - lImage->sImage->height) / 2;
        drawRect->setRect(x, y, lImage->sImage->width, lImage->sImage->height);
        showRect->setRect(0, 0, lImage->sImage->width, lImage->sImage->height);
    }
    else if(type == PREVIEW_PART)
    {
        if(clipRect->width() == 0 || clipRect->height() == 0)
            return;
        IplImage* tmpImg = cvCreateImage(cvSize(clipRect->width(), clipRect->height()), lImage->image->depth, lImage->image->nChannels);
        cvSetImageROI(lImage->image, cvRect(clipRect->x(), clipRect->y(), clipRect->width(), clipRect->height()));
        cvCopy(lImage->image, tmpImg);
        cvResetImageROI(lImage->image);

        IplImage* p = lImage->image;
        lImage->image = tmpImg;
        tmpImg = p;

        lImage->resize(this->width(), this->height());

        p = lImage->image;
        lImage->image = tmpImg;
        tmpImg = p;

        cvReleaseImage(&tmpImg);

        int x = (this->width() - lImage->sImage->width) / 2;
        int y = (this->height() - lImage->sImage->height) / 2;
        drawRect->setRect(x, y, lImage->sImage->width, lImage->sImage->height);
        showRect->setRect(0, 0, lImage->sImage->width, lImage->sImage->height);
    }
}

void Preview::setQuickMove(bool b)
{
    quickMove = b;
}
