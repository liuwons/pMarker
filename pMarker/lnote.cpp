#include "lnote.h"

LNote::LNote(QObject *parent) :
    QObject(parent)
{
}

int LNote::addMark(LMark& mark)
{
    marks.append(mark);
    return 0;
}

int LNote::addInfos(QString file, QVector<MarkInfo> infos)
{
    int i = 0;
    for(; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            break;
        }
    }
    if(i == marks.size())
    {
        marks.append(LMark(file, infos));
        return 1;
    }
    else
    {
        for(int j = 0; j < infos.size(); j ++)
        {
            if(!(marks.at(i).markinfos.contains(infos.at(j))))
            {
                LMark* l = marks.data();
                l[i].markinfos.append(infos.at(j));
            }
        }
        return 0;
    }
}

int LNote::refreshInfos(QString file, QVector<MarkInfo> infos)
{
    int i = 0;
    for(; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            break;
        }
    }
    if(i == marks.size())
    {
        marks.append(LMark(file, infos));
        return 1;
    }
    else
    {
        LMark* l = marks.data();
        l[i].markinfos.clear();
        for(int j=0; j < infos.size(); j ++)
        {
            l[i].markinfos.append(infos.at(j));
        }
        return 0;
    }
}

/**
  *清除文件名为file的文件上的所有标记
  */
int LNote::clearMarks(QString file)
{
    int i = 0;
    for(; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            LMark* l = marks.data();
            l[i].markinfos.clear();
            //marks.at(i).markinfos.clear();
        }
    }
    return 0;
}

int LNote::getMarkIndexOnFile(QString file, QRect r)
{
    int res = -1;
    int i = 0;
    for(; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            QVector<MarkInfo> m = marks.at(i).markinfos;
            for(int j = 0; j < m.size(); j ++)
            {
                if(m.at(j).rect == r)
                {
                    res = j;
                    return res;
                }
            }
        }
    }
    return res;
}

QRect LNote::getPrevRectOnFile(QString file, QRect r)
{
    int p = getMarkIndexOnFile(file, r);
    QRect res(0, 0, 0, 0);
    if(p < 0)
        return res;
    for(int i = 0; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            if(p == 0)
                return res;
            else
                return marks.at(i).markinfos.at(p - 1).rect;
        }
    }
    return res;
}

QRect LNote::getNextRectOnFile(QString file, QRect r)
{
    int p = getMarkIndexOnFile(file, r);
    QRect res(0, 0, 0, 0);
    if(p < 0)
        return res;
    for(int i = 0; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            if(p == (marks.at(i).markinfos.size() - 1))
                return res;
            else
                return marks.at(i).markinfos.at(p + 1).rect;
        }
    }
    return res;
}

int LNote::cutMarkRectOnFile(QString file, QRect r)
{
    int p = getMarkIndexOnFile(file, r);
    if(p < 0)
        return -1;
    for(int i = 0; i < marks.size(); i ++)
    {
        if(marks.at(i).fileName == file)
        {
            LMark* l = marks.data();
            l[i].markinfos.remove(p);
            return 1;
        }
    }
    return 0;
}


void LNote::write(QFile& f)
{
    QTextStream out(&f);
    for(int i = 0; i < marks.size(); i ++)
    {
        if(marks.at(i).markinfos.size() == 0)
            continue;
        out<<marks.at(i).fileName<<endl;
        out<<marks.at(i).markinfos.size()<<endl;
        QVector<MarkInfo> mfs = marks.at(i).markinfos;
        for(int j = 0; j < mfs.size(); j ++)
        {
            out<<mfs.at(j).tag<<endl;
            QRect r = mfs.at(j).rect;
            out<<QString::number(r.x())<<" ";
            out<<QString::number(r.y())<<" ";
            out<<QString::number(r.width())<<" ";
            out<<QString::number(r.height())<<endl;
        }
        //out<<endl;
    }
}
