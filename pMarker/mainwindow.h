#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSize>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QDockWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QKeySequence>
#include <QFileInfo>
#include <QFileInfoList>
#include <QByteArray>
#include <QVector>
#include <QSpacerItem>
#include <QPushButton>
#include <QFileDialog>
#include <QFrame>
#include <QScrollArea>
#include <QSlider>
#include <QLineEdit>
#include <QDialog>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QInputDialog>
#include <QToolButton>
#include <QDesktopServices>
#include <QUrl>
#include <QTextStream>
#include <QStringList>

#include "picform.h"
#include "dockform.h"
#include "markdialog.h"
#include "lmark.h"
#include "lnote.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    PicForm * picForm;

    QToolBar*  topToolBar;
    QSlider* slider;
    QToolButton* tbLeft;
    QToolButton* tbRight;
    QLabel* lb_synchro;
    QAction* clear;

    QStatusBar* statusbar;

    QLabel* lb_filename;
    QLabel* lb_pos;
    QLabel* lb_markrect;
    QLabel* lb_size;
    QLabel* lb_progress;
    QLabel* lb_scale;

    QDockWidget* markDock;
    DockForm* dockForm;

    QMenuBar*  menubar;
    QMenu* fileMenu;
    QAction* openFileAct;
    QAction* openDirAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;

    QMenu* helpMenu;
    QAction* aboutAct;
    QAction* helpAct;

    QMenu* windowMenu;
    QAction* dockedArea;

    QString* saveFileName;
    QVector<QFileInfo>* vecFile;
    QFileInfo currentFile;
    int index;                           //当前文件序号
    QString savePath;

    LNote note;

    QAction* toggleDock;

    bool saved;

    void createMenu();
    void createTopToolBar();
    void createStatusBar();
    void createMarkDock();

    void loadImage(QFileInfo info);
    void loadDir(QDir info);
    void extractDir(QDir dir, QVector<QFileInfo>* vec);

    void setProgress(int p);

    bool isPic(QFileInfo f);

public:
    MainWindow(QRect* rect, QWidget *parent = 0);
    ~MainWindow();

signals:
    void sldResizeEvent(double scale);
    void markKeyEvent();

public slots:
    void posChanged(int x, int y);
    void openFile();
    void openDir();
    void dockedWindow();
    void markEvent(QRect rect);
    void sliderValChanged(int val);
    void scaleChanged(double scale, int dir);  //鼠标滚轮操作缩放使用的槽
    void scaleChanged(double scale);           //图像数据缩放是发射的信号的接收槽
    void store();
    void save(QString fileName);
    void saveFile();
    void write();
    void nextPic();
    void prevPic();
    void clearFileMarks();

    void prevMark();
    void nextMark();
    void cutMark();

    void left();                //slider左边按钮被按下
    void right();               //slider右边按钮被按下
    void markAddedOver();       //标记添加完成，改变状态为未同步

    void about();
    void help();

    void readManifest(QDir);
    void extractManifest(QFileInfo);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
