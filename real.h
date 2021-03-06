//
//  real.h
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#ifndef __MathGraph__real__
#define __MathGraph__real__

#include <string>
#include <cmath>

// real is assumed to have implemented: unary operator: - and binary operators: +, -, *, / and the functions defined in the namespace real_functions
typedef long double real;

namespace real_functions
{
    const auto cos = static_cast<real (*)(real)>( std::cos );
    const auto sin = static_cast<real (*)(real)>( std::sin );
    const auto tan = static_cast<real (*)(real)>( std::tan );
    
    const auto arccos = static_cast<real (*)(real)>( std::acos );
    const auto arcsin = static_cast<real (*)(real)>( std::asin );
    const auto arctan = static_cast<real (*)(real)>( std::atan );
    
    const auto pow  = static_cast<real (*)(real, real)>( std::pow );
    const auto sqrt = static_cast<real (*)(real)>      ( std::sqrt );
    
    const auto ceil  = static_cast<real (*)(real)>( std::ceil );
    const auto floor = static_cast<real (*)(real)>( std::floor );
    
    const auto ln    = static_cast<real (*)(real)>( std::log );
    const auto log10 = static_cast<real (*)(real)>( std::log10 );
    
    bool parseReal(const std::string &str, long double &r);
    real round(real value, int decimals);
    std::string toString(real value);
}

#endif /* defined(__MathGraph__real__) */
