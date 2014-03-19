//
//  real.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#include "real.h"
#include <cmath>

bool real_functions::parseReal(const std::string &str, long double &real)
{
    try {
        real = stold(str);
        return true;
    }
    catch (std::exception &)
    {
        return false;
    }
}

real real_functions::round(real value, int decimals)
{
    real multiplier = std::pow(10, decimals);
    
    return static_cast<int>(value / multiplier) * multiplier;
}

std::string real_functions::toString(real value)
{
    std::string result = std::to_string(value);
    
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.') result.pop_back();
    
    return result;
}
