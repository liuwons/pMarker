#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QSize>
#include "mainwindow.h"
#include "values.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDesktopWidget* deskWidget = a.desktop();
    QRect          screenRect  = deskWidget->screenGeometry();
    screen_size   = new QSize(screenRect.width(), screenRect.height());
    window_size   = new QSize(screen_size->width() / 2, screen_size->height() / 2);

    int val = screen_size->width() > screen_size->height() ? screen_size->width() / 2 : screen_size->height() / 2;
    QRect* window_rect   = new QRect((screen_size->width() - val) / 2, (screen_size->height() - val) / 2, val, val);

    MainWindow w(window_rect);
    //w.createMarkDock();
    w.show();
    
    return a.exec();
}
