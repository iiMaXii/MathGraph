//
//  real.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#ifndef __MathGraph__real__
#define __MathGraph__real__

#include <string>

// Todo:
//  - Make typedef std::sin convenient_name_here

/**
 * real is assumed to have implemented:
 *  - Unary (-) and binary operators: +, -, *, /
 *  - The parseReal(const string &, real &)
 *  - The standard functions: std::pow, std::cos, std::sin, etc.
 *  - Probably something I forgot
 */

typedef long double real;

bool parseReal(const std::string &str, long double &real);
real round(real value, int decimals);
std::string toString(real value);

#endif /* defined(__MathGraph__real__) */
