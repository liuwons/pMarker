#ifndef MARKDIALOG_H
#define MARKDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include "values.h"

class MarkDialog : public QDialog
{
    Q_OBJECT
private:
    QDialogButtonBox* buttonBox;
    QVBoxLayout* mainLayout;
    QHBoxLayout* lytTag;
    QLabel*    lbTag;
    QLineEdit* edtTag;
    QString* strTag;

public:
    explicit MarkDialog(QWidget *parent = 0);
    
signals:
    
public slots:
    void accept();
    void reject();
    
};

#endif // MARKDIALOG_H
