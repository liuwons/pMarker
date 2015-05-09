#include "picform.h"
#include "markdialog.h"

PicForm::PicForm(QRect* rect, QWidget* parent) : QWidget(parent)
{
    setGeometry(*rect);
    setMinimumSize(rect->size());
    pressed = 0;
    lImage = 0;
    startPos.setX(0);
    startPos.setY(0);
    endPos.setX(0);
    endPos.setY(0);

    showCenter.setX(0);
    showCenter.setY(0);

    markRect = 0;
    control  = 0;
    newMark = false;

    this->setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::gray);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

PicForm::~PicForm()
{
    if(markRect != 0)
        delete markRect;
    if(lImage != 0)
        delete lImage;
}

void PicForm::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    if(lImage != 0)
    {
        QImage* img = lImage->resizedQImage();
        calcRect();
        p.drawImage(showRect, *img, imgRect);
        delete img;
        if(control == 0 && pressed == 1)
        {
            int x, y, width, height;
            x = startPos.x() < endPos.x() ? startPos.x() : endPos.x();
            y = startPos.y() < endPos.y() ? startPos.y() : endPos.y();
            width = abs(startPos.x() - endPos.x());
            height = abs(startPos.y() - endPos.y());

            p.setPen(QColor("#ff0000"));

            p.drawRect(originalToPicRect(lImage->toOriginalRect(toResizeRect(QRect(x, y, width, height)))));
            //p.drawRect(x, y, width, height);
        }
    }
    p.setPen(QColor("#ff0000"));
    if(markedInfos.size() != 0)
    {
        for(int i = 0; i < markedInfos.size(); i ++)
        {
            p.drawRect(originalToPicRect(markedInfos.at(i).rect));
        }
        //p.drawRect(originalToPicRect(markRectOnOriginal));
    }

    QWidget::paintEvent(event);
}

void PicForm::mousePressEvent(QMouseEvent *e)
{
    newMark = false;
    if(e->button() == Qt::LeftButton)
    {
        pressed = 1;

        int x = e->x();
        int y = e->y();
        x = x >= this->width() ? (this->width() - 1) : x;
        x = x < 0 ? 0 : x;
        y = y >= this->height() ? (this->height() - 1) : y;
        y = y < 0 ? 0 : y;

        startPos.setX(x);
        startPos.setY(y);
        endPos.setX(x);
        endPos.setY(y);

        //拖拽图片
        if(control != 0)
        {
            setCursor(Qt::OpenHandCursor);
        }
        //标记图片
        else
        {
            setCursor(Qt::CrossCursor);
        }
    }
    else if(e->button() == Qt::RightButton)
    {
        int x = e->x();
        int y = e->y();
        QPoint p = lImage->toOriginalPos(toResizePoint(QPoint(x, y)));
        deleteRects(p.x(), p.y());
        emit writeToData();
        emit markAdded();
        update();
    }

    QWidget::mousePressEvent(e);
}

void PicForm::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        pressed = 0;

        if(lImage == 0)
            return;

        //拖拽图片
        if(control != 0)
        {
            setCursor(Qt::OpenHandCursor);
        }
        //标记图片
        else
        {
            setCursor(Qt::CrossCursor);

            int x = e->x();
            int y = e->y();
            x = x >= this->width() ? (this->width() - 1) : x;
            x = x < 0 ? 0 : x;
            y = y >= this->height() ? (this->height() - 1) : y;
            y = y < 0 ? 0 : y;

            endPos.setX(x);
            endPos.setY(y);

            int width, height;
            x = startPos.x() < endPos.x() ? startPos.x() : endPos.x();
            y = startPos.y() < endPos.y() ? startPos.y() : endPos.y();
            width = abs(startPos.x() - endPos.x());
            height = abs(startPos.y() - endPos.y());
            if(markRect == 0)
                markRect = new QRect;
            markRect->setRect(x, y, width, height);
            newMark = true;
            markRectOnOriginal = lImage->toOriginalRect(toResizeRect(QRect(x, y, width, height)));
            emit imageMarked(markRectOnOriginal);
        }
    }
    if(e->button() == Qt::RightButton)
    {
    }

    QWidget::mouseReleaseEvent(e);
}

/*此算法在放大倍数很大时可能会由于地板数造成拖拽动作比较慢时不会产生拖拽效果，后期可以利用一个double变量记下省略的小数部分下次叠加*/
void PicForm::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);

    if(e->button() == Qt::LeftButton || e->button() == Qt::NoButton)
    {
        if(lImage == 0)
            return;

        QPoint p = toResizePoint(QPoint(e->x(), e->y()));
        p = lImage->toOriginalPos(p);
        emit cursorChanged(p.x(), p.y());

        if(pressed ==0 || lImage == 0)
            return;
        //标记图片
        if(control == 0)
        {
            setCursor(Qt::CrossCursor);
            this->update();
        }
        //拖拽图片
        else
        {
            setCursor(Qt::ClosedHandCursor);

            int ofx = endPos.x() - e->x();
            int ofy = endPos.y() - e->y();
            double scale = lImage->resizeScale;
            ofx = (int)((double)ofx / scale);
            ofy = (int)((double)ofy / scale);
            int x = showCenter.x() + ofx;
            int y = showCenter.y() + ofy;
            if(x < 0)
                x = 0;
            if(y < 0)
                y = 0;
            if(x >= lImage->image->width)
                x = lImage->image->width - 1;
            if(y >= lImage->image->height)
                y = lImage->image->height - 1;
            setCenter(QPoint(x, y));
        }
        this->update();

        endPos.setX(e->x());
        endPos.setY(e->y());
    }

}

void PicForm::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if(lImage == 0)
        return;
    calcRect();
    update();
}

void PicForm::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);

    if(lImage == 0)
        return;

    int degree = e->delta() / 8;
    int step   = degree / 15;
    double scale  = lImage->resizeScale + (double)step / 20;

    if(scale <= 0 || lImage->image->width * scale < 10 || lImage->image->height * scale < 10)
        return;
    resizeImage(scale);

    emit changeScale(scale, degree);
}

void PicForm::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Control:
        break;
    default:
        QWidget::keyPressEvent(e);
    }
}

void PicForm::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Control:
        control = 0;
        break;
    case Qt::Key_Space:
        break;
    default:
        QWidget::keyReleaseEvent(e);
    }
}

void PicForm::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
}

void PicForm::addMark()
{
    if(!newMark)
        return;
    MarkDialog* d = new MarkDialog(this);
    int t = d->exec();
    if(t == QDialog::Rejected || mark_tag == 0 || mark_tag->length() < 1)
        return;
    markedInfos.append(MarkInfo(markRectOnOriginal,*mark_tag));
    emit markAdded();
}

void PicForm::clearAllMark()
{
    markedInfos.clear();
    emit clearMarks();
    emit markAdded();
}

void PicForm::resizeImage(double scale)
{
    if(lImage == 0)
        return;
    lImage->resize(scale);
    this->update();
    emit changeScale(scale);
}

void PicForm::resizeImage(int w, int h, double* scale_buf)
{
    if(lImage == 0)
        return;
    lImage->resize(w, h);
    if(scale_buf != 0)
    {
        *scale_buf = lImage->resizeScale;
    }
    this->update();
    emit changeScale(lImage->resizeScale);
}

/**
  *计算showRect和imgRect，每次的resize和zoom操作都应该调用
  */
void PicForm::calcRect()
{
    if(lImage == 0 || lImage->sImage == 0)
        return;
    int ww = this->width();
    int wh = this->height();

    int a1, a2, b1, b2, c1, c2, d1, d2, a, b, c, d;
    a1 = ww / 2, c1 = ww - a1;
    b1 = wh / 2, d1 = wh - b1;

    a2 = (int)((double)showCenter.x() * lImage->resizeScale), c2 = lImage->sImage->width - a2;
    b2 = (int)((double)showCenter.y() * lImage->resizeScale), d2 = lImage->sImage->height - b2;

    a = a1 < a2 ? a1 : a2;
    b = b1 < b2 ? b1 : b2;
    c = c1 < c2 ? c1 : c2;
    d = d1 < d2 ? d1 : d2;

    showRect = QRect(ww / 2 - a, wh / 2 - b, a + c, b + d);
    imgRect  = QRect(a2 - a, b2 - b, a + c, b + d);
}

QSize PicForm::loadImage(const char* fileName, const LMark* lmark)
{
    markedInfos.clear();
    if(lmark != 0)
    {
        for(int i=0; i < lmark->markinfos.size(); i ++)
        {
            markedInfos.append(lmark->markinfos.at(i));
        }
    }

    int ww = this->width();
    int wh = this->height();

    if(lImage != 0)
        delete lImage;
    if(markRect != 0)
    {
        delete markRect;
        markRect = 0;
    }

    lImage = new LImage(fileName);
    if(lImage == 0)
        return QSize(0, 0);
    setCenter(QPoint(lImage->image->width / 2, lImage->image->height / 2));
    //lImage->resize(ww, wh);
    resizeImage(ww, wh, &base_resize_scale);
    //base_resize_scale = lImage->resizeScale;
    calcRect();
    this->update();

    emit changeImage(lImage->image);
    emit changeScale(lImage->resizeScale, 1);

    return QSize(lImage->image->width, lImage->image->height);
}

int PicForm::setCenter(QPoint p)
{
    showCenter = p;
    return 0;
}

//用于预览快速移动的槽
int PicForm::setCenter(int x, int y)
{
    setCenter(QPoint(x,y));
    update();
    return 0;
}

QRect PicForm::toResizeRect(QRect r)
{
    int ww = this->width();
    int wh = this->height();

    int xofs = r.x() - ww / 2;
    int yofs = r.y() - wh / 2;

    int x = lRound(showCenter.x() * lImage->resizeScale) + xofs;
    int y = lRound(showCenter.y() * lImage->resizeScale) + yofs;

    int width = r.width();
    int height = r.height();
    if(x < 0)
    {
        xofs = -x;
        x = 0;
        width -= xofs;
        if(width < 0)
            width = 0;
    }
    else if(x >= lImage->sImage->width)
    {
        x = lImage->sImage->width - 1;
        width = 0;
    }
    if(x + width > lImage->sImage->width)
    {
        //width = lImage->sImage->width - x - 1;
        width = lImage->sImage->width - x;  //不能框全图bug修复,1018
    }

    if(y < 0)
    {
        yofs = -y;
        y = 0;
        height -= yofs;
        if(height < 0)
            height = 0;
    }
    else if(y >= lImage->sImage->height)
    {
        y = lImage->sImage->height - 1;
        height = 0;
    }
    if(y + height > lImage->sImage->height)
    {
        //height = lImage->sImage->height - y - 1;
        height = lImage->sImage->height - y;  //不能框全图bug修复,1018
    }

    return QRect(x, y, width, height);
}

QPoint PicForm::toResizePoint(QPoint p)
{
    int ww = this->width();
    int wh = this->height();

    int xofs = p.x() - ww / 2;
    int yofs = p.y() - wh / 2;
    int x = (int)(showCenter.x() * lImage->resizeScale) + xofs;
    int y = (int)(showCenter.y() * lImage->resizeScale) + yofs;
    if(x < 0)
        x = 0;
    if(y < 0)
        y = 0;
    if(x >= lImage->sImage->width)
        x = lImage->sImage->width - 1;
    if(y >= lImage->sImage->height)
        y = lImage->sImage->height - 1;
    return QPoint(x, y);
}

QRect PicForm::originalToPicRect(QRect rect)
{
    int ww = this->width();
    int wh = this->height();

    int ofx = rect.x() - showCenter.x();
    int ofy = rect.y() - showCenter.y();
    ofx = lRound(ofx * lImage->resizeScale);
    ofy = lRound(ofy * lImage->resizeScale);
    int x = ww / 2 + ofx;
    int y = wh / 2 + ofy;
    int width = lRound(rect.width() * lImage->resizeScale);
    int height = lRound(rect.height() * lImage->resizeScale);
    return QRect(x, y, width, height);
}

QPoint PicForm::originalToPicPoint(QPoint p)
{
    int ww = this->width();
    int wh = this->height();

    int ofx = p.x() - showCenter.x();
    int ofy = p.y() - showCenter.y();

    ofx = lRound(ofx * lImage->resizeScale);
    ofy = lRound(ofy * lImage->resizeScale);
    int x = ww / 2 + ofx;
    int y = wh / 2 + ofy;

    return QPoint(x, y);
}

void PicForm::deleteRects(int x, int y)
{
    for(int i = 0; i < markedInfos.size(); i ++)
    {
        QRect r = markedInfos.at(i).rect;
        if(r.x() <= x && r.y() <= y && (r.x()+r.width()) >= x && (r.y()+r.height()) >= y)
        {
            markedInfos.remove(i);
            i--;
        }
    }
}

