//
//  real.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__real__
#define __MathGraph__real__

#include <string>
#include <cmath>

// Todo:
//  - Make typedef std::sin convenient_name_here

/**
 * real is assumed to have implemented:
 *  - Unary (-) and binary operators: +, -, *, /
 *  - The parseReal(const string &, real &)
 *  - The standard functions: std::pow, std::cos, std::sin, etc.
 *  - Probably something I forgot
 *  - std::ceil
 */

typedef long double real;

namespace real_functions
{
    constexpr auto cos = static_cast<real (*)(real)>( std::cos );
    constexpr auto sin = static_cast<real (*)(real)>( std::sin );
    constexpr auto tan = static_cast<real (*)(real)>( std::tan );
    
    constexpr auto arccos = static_cast<real (*)(real)>( std::acos );
    constexpr auto arcsin = static_cast<real (*)(real)>( std::asin );
    constexpr auto arctan = static_cast<real (*)(real)>( std::atan );
    
    constexpr auto pow  = static_cast<real (*)(real, real)>( std::pow  );
    constexpr auto sqrt = static_cast<real (*)(real)>      ( std::sqrt );
    
    constexpr auto ceil  = static_cast<real (*)(real)>( std::ceil  );
    constexpr auto floor = static_cast<real (*)(real)>( std::floor );
    
    constexpr auto log   = static_cast<real (*)(real)>( std::log   );
    constexpr auto log10 = static_cast<real (*)(real)>( std::log10 );
    
    bool parseReal(const std::string &str, long double &real);
    real round(real value, int decimals);
    std::string toString(real value);
}

#endif /* defined(__MathGraph__real__) */
