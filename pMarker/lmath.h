#ifndef LMATH_H
#define LMATH_H

inline int lRound(double v)
{
    if((int)v - v >= 0.5)
        return (int)v + 1;
    else
        return (int)v;
}

#endif // LMATH_H
