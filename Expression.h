//
//  Expression.h
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#ifndef __MathGraph__Expression__
#define __MathGraph__Expression__

#include "real.h"
#include <string>
#include <map>
#include <stdexcept>

class InvalidExpression : public std::invalid_argument
{
public:
    enum ErrorType {
        UNDEFINED_NAME,
        INVALID_UNARY_OPERATOR,
        PARENTHESIS_MISSMATCH,
        INVALID_CHARACTER, // == TokenReader::BAD_TOKEN
        OPERAND_UNDERFLOW,
        OPERAND_OVERFLOW
    };
private:
    typedef std::string::size_type size_type;
    
    ErrorType errorType;
    size_type position;
    size_type length;
public:
    InvalidExpression(const std::string& _what_arg, ErrorType _errorType, size_type _position, size_type _length);
    
    ErrorType getError() const;
    size_type getPosition() const;
    size_type getLength() const;
};

class Expression
{
    static std::map<std::string, const real> constants;
    static std::map<std::string, real> variables;
    static std::map<std::string, real (*)(real)> functions;
    
    std::string compiledExpression;
public:
    static bool addVariable(std::string name, real initialValue);
    static bool setVariable(std::string name, real value);
    static void addFunction(std::string name, real (*)(real));
    
    Expression();
    Expression(std::string expression);
    
    real evaluate() const;
};


#endif /* defined(__MathGraph__Expression__) */
