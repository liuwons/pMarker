#include "lmark.h"

MarkInfo::MarkInfo(QRect r, QString t)
{
    rect = r;
    tag = t;
}

MarkInfo::MarkInfo()
{
    rect = QRect(0, 0, 0, 0);
    tag = QString();
}

bool MarkInfo::operator ==(const MarkInfo& m)
{
    return (m.rect == this->rect);
}

LMark::LMark()
{
}

LMark::LMark(QString f)
{
    fileName = f;
}

LMark::LMark(QString f, QVector<MarkInfo> i)
{
    fileName = f;
    markinfos = i;
}

LMark::~LMark()
{
}

int LMark::addMark(MarkInfo& m)
{
    for(int i = 0; i < markinfos.size(); i ++)
    {
        if(markinfos.at(i).rect == m.rect)
        {
            return 0;
        }
    }
    markinfos.append(m);
    return 1;
}
