#include "dockform.h"

DockForm::DockForm(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout;

    preview_img = new Preview(*screen_size, Preview::PREVIEW_OVERALL);
    preview_mrk = new Preview(*screen_size, Preview::PREVIEW_PART);
    preview_img->setMaximumHeight(screen_size->width() / 8);
    preview_img->setMaximumWidth(screen_size->width() / 8);
    preview_mrk->setMaximumHeight(screen_size->width() / 8);
    preview_mrk->setMaximumWidth(screen_size->width() / 8);
    preview_img->setMinimumSize(screen_size->width() / 8, screen_size->width() / 8);
    preview_mrk->setMinimumSize(screen_size->width() / 8, screen_size->width() / 8);

    tb_next = new QToolButton;
    tb_next->setIcon(QIcon("res/playnext3.png"));
    tb_next->setIconSize(QSize(48,48));
    tb_next->setAutoRaise(true);
    tb_prev = new QToolButton;
    tb_prev->setIcon(QIcon("res/playprev3.png"));
    tb_prev->setIconSize(QSize(48,48));
    tb_prev->setAutoRaise(true);
    tb_write = new QToolButton;
    tb_write->setIcon(QIcon("res/write.png"));
    tb_write->setIconSize(QSize(48,48));
    tb_write->setAutoRaise(true);
    lyt_tools = new QHBoxLayout;
    lyt_tools->addWidget(tb_prev);
    lyt_tools->addWidget(tb_write);
    lyt_tools->addWidget(tb_next);

/*
    tb_next2 = new QToolButton;
    tb_next2->setIcon(QIcon("res/Arrows-Right-round-icon.png"));
    tb_next2->setIconSize(QSize(48,48));
    tb_next2->setAutoRaise(true);
    tb_prev2 = new QToolButton;
    tb_prev2->setIcon(QIcon("res/Arrows-left-round-icon.png"));
    tb_prev2->setIconSize(QSize(48,48));
    tb_prev2->setAutoRaise(true);
    tb_cut = new QToolButton;
    tb_cut->setIcon(QIcon("res/cut.png"));
    tb_cut->setIconSize(QSize(48,48));
    tb_cut->setAutoRaise(true);
    lyt_tools2 = new QHBoxLayout;
    lyt_tools2->addWidget(tb_prev2);
    lyt_tools2->addWidget(tb_cut);
    lyt_tools2->addWidget(tb_next2);*/

    mainLayout->addWidget(preview_img);
    mainLayout->addLayout(lyt_tools);
    mainLayout->addWidget(preview_mrk);
    //mainLayout->addLayout(lyt_tools2);

    setLayout(mainLayout);
}

DockForm::~DockForm()
{
    if(preview_img != 0)
        delete preview_img;
    if(preview_mrk != 0)
        delete preview_mrk;
    if(mainLayout != 0)
        delete mainLayout;
}
