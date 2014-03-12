//
//  DEBUG.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__DEBUG___
#define __MathGraph__DEBUG___

#ifdef DEBUG
#include <iostream>
#define LOG_DEBUG( str ) do { std::cout << str << std::endl; } while( 0 )
#else
#define LOG_DEBUG( str )
#endif

#endif /* defined(__MathGraph__DEBUG___) */
