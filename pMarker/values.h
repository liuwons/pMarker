#ifndef VALUES_H
#define VALUES_H

#include <QSize>
#include <QRectF>
#include <QString>

extern QSize* screen_size;
extern QSize* window_size;

extern int client_width;         //mainWindow�հ������
extern int client_height;        //mainWindow�հ����߶�

extern int control;
extern int pressed;

extern int slider_length;
extern double slider_val[];

extern double base_resize_scale;

extern QString* mark_tag;

#endif // VALUES_H
