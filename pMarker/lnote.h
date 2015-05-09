#ifndef LNOTE_H
#define LNOTE_H

#include <QObject>
#include <QTextStream>

#include "lmark.h"

class LNote : public QObject
{
    Q_OBJECT
public:
    QVector<LMark> marks;

    explicit LNote(QObject *parent = 0);

    int addMark(LMark& mark);
    int addInfos(QString file, QVector<MarkInfo> infos);
    int refreshInfos(QString file, QVector<MarkInfo> infos);
    int clearMarks(QString file);

    int getMarkIndexOnFile(QString file, QRect r);
    QRect getPrevRectOnFile(QString file, QRect r);
    QRect getNextRectOnFile(QString file, QRect r);
    int cutMarkRectOnFile(QString file, QRect r);

    void write(QFile& f);
    
signals:
    
public slots:
    
};

#endif // LNOTE_H
