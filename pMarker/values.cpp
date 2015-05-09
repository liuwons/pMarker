#include <QSize>
#include <QRectF>
#include <QString>

QSize* screen_size = 0;
QSize* window_size = 0;

int client_width = 0;
int client_height = 0;
int control = 0;
int pressed = 0;

double base_resize_scale = 0;

//int slider_length = 20;
//double slider_val[20] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};

int slider_length = 40;
double slider_val[40] = {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
                         0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0,
                         1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
                         2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0};

QString* mark_tag = 0;
