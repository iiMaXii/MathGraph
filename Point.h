//
//  Point.h
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#ifndef __MathGraph__Point__
#define __MathGraph__Point__

#include "real.h"

template<typename T>
class Point
{
    T x, y;
    
public:
    Point(T _x, T _y);
    
    T getX() const;
    T getY() const;
    
    void setX(T _x);
    void setY(T _y);
};

template<typename T>
Point<T>::Point(T _x, T _y) : x(_x), y(_y) {}

template<typename T>
T Point<T>::getX() const
{
    return x;
}

template<typename T>
T Point<T>::getY() const
{
    return y;
}

template<typename T>
void Point<T>::setX(T _x)
{
    x = _x;
}

template<typename T>
void Point<T>::setY(T _y)
{
    y = _y;
}

#endif /* defined(__MathGraph__Point__) */
