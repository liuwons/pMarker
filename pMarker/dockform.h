#ifndef DOCKFORM_H
#define DOCKFORM_H

#include <QWidget>
#include <QSize>
#include <QRect>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QToolButton>

#include "preview.h"
#include "values.h"

class DockForm : public QWidget
{
    Q_OBJECT

public:
    QVBoxLayout* mainLayout;

    QToolButton* tb_next;
    QToolButton* tb_prev;
    QToolButton* tb_write;
    QHBoxLayout* lyt_tools;
/*
    QToolButton* tb_next2;
    QToolButton* tb_prev2;
    QToolButton* tb_cut;
    QHBoxLayout* lyt_tools2;*/

    Preview* preview_img;
    Preview* preview_mrk;

    explicit DockForm(QWidget *parent = 0);
    ~DockForm();
    
signals:
    
public slots:
    
};

#endif // DOCKFORM_H
