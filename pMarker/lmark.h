#ifndef LMARK_H
#define LMARK_H

#include <QFileInfo>
#include <QString>
#include <QRect>
#include <QVector>

class MarkInfo
{
public:
    QRect rect;
    QString tag;

    MarkInfo();
    MarkInfo(QRect r, QString t);

    bool operator ==(const MarkInfo& m);
};

/**
  *一个文件的标记集
  */
class LMark
{
public:
    QString fileName;
    QVector<MarkInfo> markinfos;

    LMark();
    LMark(QString f);
    LMark(QString f, QVector<MarkInfo> i);
    ~LMark();

    int addMark(MarkInfo& m);

};

#endif // LMARK_H
