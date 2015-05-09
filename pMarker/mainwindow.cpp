#include "mainwindow.h"
#include "values.h"

MainWindow::MainWindow(QRect* rect, QWidget *parent)
    : QMainWindow(parent)
{
    index = 0;
    vecFile = 0;
    saveFileName = 0;
    saved = true;

    savePath = "./";

    setWindowIcon(QIcon("res/camera.png"));
    setFocusPolicy(Qt::ClickFocus);

    createMenu();
    createStatusBar();
    this->setGeometry(*rect);
    setMinimumSize(rect->size());

    picForm = new PicForm(new QRect(0, 0, this->width(), this->height()));
    picForm->setMinimumSize(screen_size->width() / 4, screen_size->height() / 3);
    this->setCentralWidget(picForm);

    createTopToolBar();
    createMarkDock();

    connect(picForm, SIGNAL(cursorChanged(int,int)), this, SLOT(posChanged(int,int)));
    connect(picForm, SIGNAL(imageMarked(QRect)), this, SLOT(markEvent(QRect)));
    connect(this, SIGNAL(markKeyEvent()), picForm, SLOT(addMark()));
    connect(picForm, SIGNAL(markAdded()), this, SLOT(markAddedOver()));
}


MainWindow::~MainWindow()
{
    
}

void MainWindow::createMenu()
{
    menubar = this->menuBar();

    fileMenu = menubar->addMenu(tr("File"));
    openFileAct = new QAction(tr("Open File"), this);
    openFileAct->setIcon(QIcon("res/file.png"));
    openFileAct->setShortcut(QKeySequence::Open);
    openDirAct = new QAction(tr("Open Folder"), this);
    openDirAct->setIcon(QIcon("res/folder.png"));
    saveAct = new QAction(tr("Save"), this);
    saveAct->setIcon(QIcon("res/save_file2.png"));
    saveAct->setShortcut(QKeySequence::Save);
    saveAsAct = new QAction(tr("Save As"), this);
    saveAsAct->setIcon(QIcon("res/save_as2.png"));
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    exitAct = new QAction(tr("Exit"), this);
    exitAct->setIcon(QIcon("res/exit2.png"));
    exitAct->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(openDirAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menubar->addMenu(tr("Help"));
    aboutAct = new QAction(tr("About"), this);
    aboutAct->setIcon(QIcon("res/about.png"));
    helpAct = new QAction(tr("Help"), this);
    helpAct->setIcon(QIcon("res/help.png"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(helpAct);

    windowMenu = menubar->addMenu(tr("Window"));
    dockedArea = new QAction(tr("Docked Widget"), this);
    dockedArea->setIcon(QIcon("res/visual.png"));
    windowMenu->addAction(dockedArea);

    connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(openDirAct, SIGNAL(triggered()), this, SLOT(openDir()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    connect(dockedArea, SIGNAL(triggered()), this, SLOT(dockedWindow()));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(write()));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void MainWindow::createTopToolBar()
{
    topToolBar = new QToolBar("top toolbar");
    clear = new QAction(tr("Clear"), this);
    clear->setIcon(QIcon("res/broom.png"));
    topToolBar->addAction(clear);
    topToolBar->addSeparator();
    lb_synchro =new QLabel;
    lb_synchro->setPixmap(QPixmap("res/security_refresh24.png"));
    saved = true;
    topToolBar->addWidget(lb_synchro);
    topToolBar->addSeparator();

    tbLeft = new QToolButton;
    tbLeft->setIcon(QIcon("res/zoomout24.png"));
    topToolBar->addWidget(tbLeft);
    slider = new QSlider(Qt::Horizontal);
    slider->setStyleSheet("QSlider::groove:horizontal{"
                          "height 2px;"
                          "left: 12px; right: 12px;"
                          "margin: 0 0 0 0;}"
                          "QSlider::handle:horizontal{"
                          "background: #ffffff;"
                          "margin: -7px -7px -7px -7px;"
                          "width: 24px;}"
                          "QSlider::sub-page:horizontal{"
                          "background: #606060;}"
                          "QSlider::add-page:horizontal{"
                          "background: #c0c0c0;}");
    topToolBar->addWidget(slider);
    slider->setMaximum(slider_length - 1);
    slider->setMinimum(0);
    slider->setSingleStep(1);
    slider->setMaximumWidth(this->width() / 4);
    //slider->setTickPosition(QSlider::TicksBelow);
    slider->setValue(slider->maximum() / 2);
    tbRight = new QToolButton;
    tbRight->setIcon(QIcon("res/zoomin24.png"));
    topToolBar->addWidget(tbRight);
    connect(tbLeft, SIGNAL(clicked()), this, SLOT(left()));
    connect(tbRight, SIGNAL(clicked()), this, SLOT(right()));

    lb_scale = new QLabel;
    topToolBar->addWidget(lb_scale);
    topToolBar->addSeparator();

    addToolBar(Qt::TopToolBarArea,topToolBar);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValChanged(int)));
    if(picForm != 0)
    {
        connect(this, SIGNAL(sldResizeEvent(double)), picForm, SLOT(resizeImage(double)));
        connect(clear, SIGNAL(triggered()), picForm, SLOT(clearAllMark()));
        connect(picForm, SIGNAL(changeScale(double, int)), this, SLOT(scaleChanged(double, int)));
        connect(picForm, SIGNAL(changeScale(double)), this, SLOT(scaleChanged(double)));
        connect(picForm, SIGNAL(clearMarks()), this, SLOT(clearFileMarks()));
        connect(picForm, SIGNAL(writeToData()), this, SLOT(store()));
    }
}

void MainWindow::createStatusBar()
{
    statusbar = statusBar();
    lb_markrect = new QLabel;
    lb_filename = new QLabel;
    lb_pos = new QLabel("");
    //lb_pos->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    lb_size = new QLabel("");
    lb_progress = new QLabel("");
    lb_progress->setAlignment(Qt::AlignRight);
    statusbar->addWidget(lb_filename, 1);
    statusbar->addWidget(lb_pos, 1);
    statusbar->addWidget(lb_markrect, 1);
    statusbar->addWidget(lb_size, 1);
    statusbar->addWidget(lb_progress, 0);
}

void MainWindow::createMarkDock()
{
    markDock = new QDockWidget(tr("Mark"));
    markDock->setMinimumWidth(screen_size->width() / 7);
    markDock->setObjectName("markDockWidget");
    dockForm = new DockForm;
    markDock->setWidget(dockForm);
    markDock->setAllowedAreas(Qt::RightDockWidgetArea);
    markDock->setFloating(true);
    addDockWidget(Qt::RightDockWidgetArea, markDock);

    if(picForm->lImage != 0)
    {
        dockForm->preview_img->setImage(picForm->lImage->image);
        dockForm->preview_mrk->setImage(picForm->lImage->image);
    }

    dockForm->preview_img->setQuickMove(true);

    connect(picForm, SIGNAL(changeImage(IplImage*)), dockForm->preview_img, SLOT(setImage(IplImage*)));
    connect(picForm, SIGNAL(changeImage(IplImage*)), dockForm->preview_mrk, SLOT(setImage(IplImage*)));
    connect(picForm, SIGNAL(imageMarked(QRect)), dockForm->preview_mrk, SLOT(setShowRect(QRect)));
    connect(dockForm->preview_img, SIGNAL(quickMoveEvent(int,int)), picForm, SLOT(setCenter(int, int)));
    connect(dockForm->tb_prev, SIGNAL(clicked()), this, SLOT(prevPic()));
    connect(dockForm->tb_next, SIGNAL(clicked()), this, SLOT(nextPic()));
    connect(dockForm->tb_write, SIGNAL(clicked()), this, SLOT(write()));

    //connect(dockForm->tb_prev2, SIGNAL(clicked()), this, SLOT(prevMark()));
    //connect(dockForm->tb_next2, SIGNAL(clicked()), this, SLOT(nextMark()));
    //connect(dockForm->tb_cut, SIGNAL(clicked()), this, SLOT(cutMark()));

}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!saved)
    {
        QMessageBox msgBox;
        msgBox.setIconPixmap(QPixmap("res/warn.png"));
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
           case QMessageBox::Save:
               write();
               break;
           case QMessageBox::Discard:
               break;
           case QMessageBox::Cancel:
               e->ignore();
               break;
           default:
               // should never be reached
               break;
         }
    }
    //QMainWindow::closeEvent(e);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QMainWindow::paintEvent(e);
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    window_size->setWidth(e->size().width());
    window_size->setHeight(e->size().height());
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    //bool edt_has_focus = dockForm == 0 || dockForm->edt_tag == 0 || dockForm->edt_tag->hasFocus();
    QWidget::keyPressEvent(e);
    switch(e->key())
    {
    case Qt::Key_Control:
        control = 1;
        picForm->setCursor(Qt::OpenHandCursor);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    bool edt_has_focus = (dockForm == 0);
    QWidget::keyReleaseEvent(e);
    switch(e->key())
    {
    case Qt::Key_Control:
        control = 0;
        picForm->setCursor(Qt::CrossCursor);
        break;
    case Qt::Key_D:
        if(!edt_has_focus)
            nextPic();
        break;
    case Qt::Key_A:
        if(!edt_has_focus)
            prevPic();
        break;
    case Qt::Key_Space:
        //qDebug()<<"space"<<endl;
        emit markKeyEvent();
        store();
        break;
    }
}

void MainWindow::posChanged(int x, int y)
{
    QString pos("");
    pos += QString::number(x);
    pos += ",";
    pos += QString::number(y);
    lb_pos->setText(pos);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.length() == 0)
    {
        return;
    }
    note.marks.clear();
    if(saveFileName != 0)
    {
        delete saveFileName;
        saveFileName = 0;
    }
    savePath = ".\\";
    QFileInfo info(fileName);
    if(vecFile != 0)
        delete vecFile;
    vecFile = new QVector<QFileInfo>;
    vecFile->append(info);
    loadImage(info);

    index = 1;
    setProgress(index);
}

void MainWindow::openDir()
{
    //QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    qDebug()<<dirName<<":"<<dirName.length()<<endl;
    if(dirName.length() == 0)
    {
        //qDebug()<<"empty"<<endl;
        return;
    }
    if(saveFileName != 0)
    {
        delete saveFileName;
        saveFileName = 0;
    }
    note.marks.clear();
    savePath = dirName.append("\\");
    if(dirName.length() == 0)
        return;
    QDir dir(dirName);
    loadDir(dir);
    setWindowTitle(dirName);

    index = 1;
    setProgress(index);
}

void MainWindow::loadImage(QFileInfo info)
{
    QString fileName = info.fileName();
    lb_filename->setText(fileName);

    currentFile = info;

    QByteArray ba = info.absoluteFilePath().toLocal8Bit();
    char* imgName = ba.data();
    QSize size;

    QString fname = currentFile.absoluteFilePath();
    int i;
    for(i = 0; i < note.marks.size(); i ++)
    {
        if(fname == note.marks.at(i).fileName)
            break;
    }
    if(i == note.marks.size())
    {
        size = picForm->loadImage(imgName);
    }
    else
    {
        const LMark* lm = &(note.marks.at(i));
        size = picForm->loadImage(imgName, lm);
    }

    //QSize size = picForm->loadImage(imgName);

    QString str("");
    str += QString::number(size.width());
    str += "*";
    str += QString::number(size.height());
    lb_size->setText(str);
}

void MainWindow::loadDir(QDir dir)
{
    if(vecFile != 0)
    {
        delete vecFile;
        vecFile = 0;
    }
    vecFile = new QVector<QFileInfo>;
    extractDir(dir, vecFile);
    if(vecFile->size() > 0)
    {
        //read the manifest
        readManifest(dir);
        currentFile = vecFile->at(0);
        loadImage(currentFile);
    }
    else
    {

    }
}

void MainWindow::readManifest(QDir dir)
{
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
    for(int i = 0; i < list.size(); i ++)
    {
        QFileInfo f = list.at(i);
        QString fn = f.fileName();
        if(fn == "mark.txt")
        {
            extractManifest(f);
        }
    }
}

void MainWindow::extractManifest(QFileInfo fi)
{
    QFile f(fi.absoluteFilePath());
    if(!(f.open(QIODevice::ReadWrite)))
        return;
    QTextStream in(&f);
    while(!(in.atEnd()))
    {
        QString fileName = in.readLine();
        qDebug()<<"file name:"<<fileName;
        note.marks.append(LMark(fileName));
        int n = in.readLine().toInt();
        qDebug()<<"n:"<<n;
        for(int i = 0; i < n; i ++)
        {
            QString mark = in.readLine();
            QString rect = in.readLine();
            QStringList nums = rect.split(" ");
            int x = nums.at(0).toInt();
            int y = nums.at(1).toInt();
            int w = nums.at(2).toInt();
            int h = nums.at(3).toInt();
            //note.marks.at(note.marks.size() - 1).markinfos.append(MarkInfo(QRect(x, y, w, h), mark));
            LMark* l = note.marks.data();
            l[note.marks.size() - 1].markinfos.append(MarkInfo(QRect(x, y, w, h), mark));
            qDebug()<<"mark:"<<mark;
            qDebug()<<"rect:"<<x<<" "<<y<<" "<<w<<" "<<h;
        }
    }
    f.close();
}

void MainWindow::extractDir(QDir dir, QVector<QFileInfo>* vec)
{
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    for(int i = 0; i < list.size(); i ++)
    {
        QFileInfo f = list.at(i);
        if(f.isDir())
        {
            extractDir(QDir(f.absoluteFilePath()), vec);
        }
        else if(f.isFile() && isPic(f))
        {
            vec->append(f);
        }
    }
}

bool MainWindow::isPic(QFileInfo f)
{
    QString name = f.fileName();
    if(name.endsWith("jpg") || name.endsWith("bmp") || name.endsWith("png"))
        return true;
    else
        return false;
}

void MainWindow::nextPic()
{
    //store();
    if(vecFile == 0 || vecFile->size() == 0)
        return;
    if(index == vecFile->size())
        return;
    currentFile = vecFile->at(index);
/*
    QString fname = currentFile.absoluteFilePath();
    int i;
    for(i = 0; i < note.marks.size(); i ++)
    {
        if(fname == note.marks.at(i).fileName)
            break;
    }
    if(i == note.marks.size())
    {
        loadImage(currentFile);
    }
    else
    {
        const LMark* lm = &(note.marks.at(i));
        loadImage(currentFile, lm);
    }*/

    loadImage(currentFile);
    index ++;
    setProgress(index);
}

void MainWindow::prevPic()
{
    //store();
    if(vecFile == 0 || vecFile->size() == 0)
        return;
    if(index == 1)
        return;
    index --;
    currentFile = vecFile->at(index - 1);
/*
    QString fname = currentFile.absoluteFilePath();
    int i;
    for(i = 0; i < note.marks.size(); i ++)
    {
        if(fname == note.marks.at(i).fileName)
            break;
    }
    if(i == note.marks.size())
    {
        loadImage(currentFile);
    }
    else
    {
        const LMark* lm = &(note.marks.at(i));
        loadImage(currentFile, lm);
    }*/

    loadImage(currentFile);
    setProgress(index);
}

void MainWindow::setProgress(int p)
{
    if(p > vecFile->size())
        return;
    QString strP("");
    strP += QString::number(p);
    strP += "/";
    strP += QString::number(vecFile->size());
    lb_progress->setText(strP);
}

void MainWindow::dockedWindow()
{
    delete markDock;
    createMarkDock();
}

void MainWindow::markEvent(QRect rect)
{
    QString s("(");
    s += QString::number(rect.x());
    s += ",";
    s += QString::number(rect.y());
    s += ",";
    s += QString::number(rect.width());
    s += ",";
    s += QString::number(rect.height());
    s += ")";
    lb_markrect->setText(s);
}

void MainWindow::sliderValChanged(int val)
{
    double scale = slider_val[val] * base_resize_scale;
    emit sldResizeEvent(scale);
}

void MainWindow::scaleChanged(double scale, int dir)
{
    int i = 0;
    if(base_resize_scale == 0)
    {
        i = slider_length / 2;
    }
    else
    {
        double val = scale / base_resize_scale;
        if(dir > 0)
        {
            i = 0;
            while(i < (slider_length - 1) && slider_val[i + 1] <= val)
            {
                i ++;
            }
        }
        else if(dir < 0)
        {
            i = slider_length - 1;
            while(i > 0 && slider_val[i - 1] >= val)
            {
                i --;
            }
        }

    }
    if(slider != 0)
    {
        slider->setValue(i);
    }

}

void MainWindow::scaleChanged(double scale)
{
    //qDebug()<<base_resize_scale<<" "<<scale<<endl;
    QString str = QString::number(scale / base_resize_scale, 'f', 2);
    if(lb_scale != 0)
        lb_scale->setText(str);
}

/**
  *将picForm中已经标注的标记写入全局数据结构note
  */
void MainWindow::store()
{
    //note.addInfos(currentFile.absoluteFilePath(), picForm->markedInfos);
    note.refreshInfos(currentFile.absoluteFilePath(), picForm->markedInfos);
}

void MainWindow::save(QString fileName)
{
    QFile f(fileName);
    if(!f.open(QIODevice::ReadWrite | QFile::Text | QIODevice::Truncate))
    {
        QMessageBox mb(this);
        mb.setText(tr("open file failed !"));
        mb.exec();
        return;
    }
    if(saveFileName != 0)
        delete saveFileName;
    saveFileName = new QString(fileName);
    note.write(f);
    f.flush();
    f.close();
    lb_synchro->setPixmap(QPixmap("res/security_refresh24.png"));
    saved = true;
}

void MainWindow::saveFile()
{
    bool ok;
    /*QString fileName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                      tr("file name:"), QLineEdit::Normal,
                                      QDir::home().dirName(), &ok);*/
    QString fileName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("file name:"), QLineEdit::Normal,
                                             "mark.txt", &ok);
    if (ok && !fileName.isEmpty())
    {
        save(savePath+fileName);
    }
}

void MainWindow::write()
{
    if(saveFileName == 0)
    {
        saveFile();
    }
    else
    {
        save(*saveFileName);
    }
}

void MainWindow::left()
{
    if(picForm == 0 || picForm->lImage == 0)
        return;
    int v = slider->value();
    if(v > slider->minimum())
        v --;
    slider->setValue(v);
}

void MainWindow::right()
{
    if(picForm == 0 || picForm->lImage == 0)
        return;
    int v = slider->value();
    if(v < slider->maximum())
        v ++;
    slider->setValue(v);
}

void MainWindow::markAddedOver()
{
    lb_synchro->setPixmap(QPixmap("res/security_warning24.png"));
    saved = false;
}


void MainWindow::clearFileMarks()
{
    note.clearMarks(currentFile.absoluteFilePath());
}

void MainWindow::about()
{
    QUrl url = QUrl::fromLocalFile("about.txt");
    QDesktopServices::openUrl(url);
}

void MainWindow::help()
{
    QUrl url = QUrl::fromLocalFile("help.txt");
    QDesktopServices::openUrl(url);
}

void MainWindow::prevMark()
{
    QRect r = note.getPrevRectOnFile(currentFile.absoluteFilePath(), *(dockForm->preview_mrk->clipRect));
    if(r == QRect(0, 0, 0, 0))
    {
        return;
    }
    dockForm->preview_mrk->setShowRect(r);
}

void MainWindow::nextMark()
{
    QRect r = note.getNextRectOnFile(currentFile.absoluteFilePath(), *(dockForm->preview_mrk->clipRect));
    if(r == QRect(0, 0, 0, 0))
    {
        return;
    }
    dockForm->preview_mrk->setShowRect(r);
}

void MainWindow::cutMark()
{
    for(int i = 0; i < picForm->markedInfos.size(); i ++)
    {
        if(picForm->markedInfos.at(i).rect == *(dockForm->preview_mrk->clipRect))
            picForm->markedInfos.remove(i);
    }
    note.cutMarkRectOnFile(currentFile.absoluteFilePath(), *(dockForm->preview_mrk->clipRect));
}

