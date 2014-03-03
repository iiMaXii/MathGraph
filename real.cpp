//
//  real.cpp
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#include "real.h"
#include <cmath>

bool parseReal(const std::string &str, long double &real)
{
    try {
        real = stold(str);
        return true;
    } catch (std::exception &) {
        return false;
    }
}

real round(real value, int decimals)
{
    real multiplier = std::pow(10, decimals);
    
    return static_cast<int>(value / multiplier) * multiplier;
}

std::string toString(real value)
{
    std::string result = std::to_string(value);
    result.erase (result.find_last_not_of('0') + 1, std::string::npos);
    
    if (result.back() == '.') result.pop_back();
    
    return result;
}