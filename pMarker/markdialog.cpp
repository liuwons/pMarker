#include "markdialog.h"

MarkDialog::MarkDialog(QWidget *parent) :
    QDialog(parent)
{
    strTag = 0;

    mainLayout = new QVBoxLayout;
    lytTag = new QHBoxLayout;
    lbTag = new QLabel(tr("tag:"));
    edtTag = new QLineEdit;
    edtTag->setText("0");
    edtTag->selectAll();
    lytTag->addWidget(lbTag);
    lytTag->addWidget(edtTag);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addLayout(lytTag);
    mainLayout->addWidget(buttonBox);
    this->setLayout(mainLayout);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void MarkDialog::accept()
{
    strTag = new QString;
    *strTag = edtTag->text();
    mark_tag = strTag;
    QDialog::accept();
}

void MarkDialog::reject()
{
    QDialog::reject();
}
