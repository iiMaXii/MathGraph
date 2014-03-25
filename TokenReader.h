//
//  TokenReader.h
//  MathGraph
//
//  Copyright Max Ekström. Licenced under GPL v3 (see README).
//
//

#ifndef __MathGraph__TokenReader__
#define __MathGraph__TokenReader__

#include <sstream>

enum TokenType {
    NUMBER,
    OPERATOR,
    NAME,
    PARENTHESIS_START,
    PARENTHESIS_END,
    END,
    BAD_TOKEN
};

class TokenReader
{
    std::istringstream expressionStream;
    
public:
    TokenReader(std::string expression);
    
    TokenType read(std::string &result);
    size_t getCurrentPosition();
    
    static bool isOperator(const std::string &token);
    static bool isUnary(const std::string &token);
    static bool isLeftAssociative(const std::string &token);
    static int operatorPrecedence(const std::string &token);
    static bool isName(const std::string &token);
};

#endif /* defined(__MathGraph__TokenReader__) */
