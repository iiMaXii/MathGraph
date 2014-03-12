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

//namespace real_f
//{
	//real(*sin_ff)(real);
	//real(*sinn_ff)(real) = static_cast<real(*)(real)>(std::sin);

	//real (*pow)(real, real) = std::pow;

	//real (*sin)(real) = std::sin;
	//real (*cos)(real) = std::cos;
	//real (*tan)(real) = std::tan;
//}

bool parseReal(const std::string &str, long double &real);
real round(real value, int decimals);
std::string toString(real value);

#endif /* defined(__MathGraph__real__) */
