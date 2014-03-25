//
//  Expression.cpp
//  MathGraph
//
//  Copyright Max Ekström. Licensed under GPL v3 (see README).
//
//

#include "Expression.h"
#include "TokenReader.h"

#include <iostream>
#include <stack>
#include <queue>

InvalidExpression::InvalidExpression(const std::string &_what_arg, ErrorType _errorType, size_type _position, size_type _length)
    : invalid_argument(_what_arg),
      errorType(_errorType),
      position(_position),
      length(_length)
{}

InvalidExpression::ErrorType InvalidExpression::getError() const
{
    return errorType;
}

InvalidExpression::size_type InvalidExpression::getPosition() const
{
    return position;
}

InvalidExpression::size_type InvalidExpression::getLength() const
{
    return length;
}

// ---------------- BEGIN STATIC ---------------- //

std::map<std::string, const real> Expression::constants = {
    {"pi", 3.141592653589793238462643383279502884197169399375105820974944},
    {"e",  2.718281828459045235360287471352662497757247093699959574966967}
};

std::map<std::string, real> Expression::variables;

std::map<std::string, real (*)(real)> Expression::functions = {
    {"sin", real_functions::sin},
    {"cos", real_functions::cos},
    {"tan", real_functions::tan},
    {"arcsin", real_functions::arcsin},
    {"arccos", real_functions::arccos},
    {"arctan", real_functions::arctan},
    {"sqrt", real_functions::sqrt},
    {"floor", real_functions::floor},
    {"ceil", real_functions::ceil},
    {"ln", real_functions::ln},
    {"log", real_functions::log10}
};

bool Expression::addVariable(std::string name, real initialValue)
{
    bool wasCreated = false;
    
    if (variables.find(name) == variables.end())
    {
        variables[name] = initialValue;
        
        wasCreated = true;
    }
    
    return wasCreated;
}

bool Expression::setVariable(std::string name, real value)
{
    bool exists = false;
    
    if (variables.find(name) != variables.end())
    {
        variables[name] = value;
        
        exists = true;
    }
    
    return exists;
}

void Expression::addFunction(std::string name, real (*functionPointer)(real))
{
    functions[name] = functionPointer;
}

// ----------------- END STATIC ----------------- //

Expression::Expression() {}

Expression::Expression(std::string expression)
    : Expression()
{
    std::stack<std::string> tempStack;
    std::queue<std::string> outputQueue;
    
    TokenReader r(expression);
    std::string token;
    
    bool isUnary = true;
    int numOperands = 0;
    //bool expectsParenthesis = false;
    //bool implicitMultiplication = false;
    
    TokenType tokenType = r.read(token);
    while (tokenType != END)
    {
        switch (tokenType)
        {
            case NUMBER:
                outputQueue.push(token); // Add multiplication if next is '(', number, variable
                
                isUnary = false;
                ++numOperands;
                break;
            case NAME:
                if (constants.find(token) != constants.end())
                { // constant
                    outputQueue.push(real_functions::toString(constants[token])); // Add multiplication if next is '(', number, variable
                    
                    isUnary = false;
                    ++numOperands;
                }
                else if(variables.find(token) != variables.end())
                { // variable
                    outputQueue.push(token); // Add multiplication if next is '(', number, variable
                    
                    isUnary = false;
                    ++numOperands;
                }
                else if (functions.find(token) != functions.end())
                { // function
                    tempStack.push(token);
                    //expectsParenthesis = true;
                    // Syntax error if parenthesises is NOT found
                }
                else
                {
                    throw InvalidExpression("Undefined name", InvalidExpression::UNDEFINED_NAME, r.getCurrentPosition() - token.length(), token.length());
                }
                break;
            case OPERATOR:
                if (isUnary)
                {
                    if (token == "-")
                    {
                        tempStack.push("~"); // Unary minus
                    }
                    else if (token == "+")
                    {
                        // do nothing
                    }
                    else
                    {
                        throw InvalidExpression("Invalid unary operator", InvalidExpression::INVALID_UNARY_OPERATOR, r.getCurrentPosition() - 1, 1);
                    }
                }
                else
                {
                    while (!tempStack.empty() && TokenReader::isOperator(tempStack.top()) &&
                           (
                            (TokenReader::isLeftAssociative(token) && TokenReader::operatorPrecedence(token) == TokenReader::  operatorPrecedence(tempStack.top())) ||
                            TokenReader::operatorPrecedence(token) < TokenReader::operatorPrecedence(tempStack.top())
                            )
                           )
                    {
                        outputQueue.push(tempStack.top());
                        tempStack.pop();
                    }
                    
                    tempStack.push(token);
                    
                    --numOperands;
                }
                
                isUnary = true;
                break;
            case PARENTHESIS_START:
                tempStack.push(token);
                
                //expectsParenthesis = false;
                isUnary = true;
                break;
            case PARENTHESIS_END:
                if (tempStack.empty())
                    throw InvalidExpression("Parenthesis missmatch", InvalidExpression::PARENTHESIS_MISSMATCH, r.getCurrentPosition() - 1, 1);
                
                while (!tempStack.empty() && tempStack.top() != "(")
                {
                    outputQueue.push(tempStack.top());
                    tempStack.pop();
                }
                
                tempStack.pop(); // Discard '('
                
                if (!tempStack.empty() && functions.find(tempStack.top()) != functions.end())
                {
                    outputQueue.push(tempStack.top());
                    tempStack.pop();
                }
                
                isUnary = false;
                break;
            case BAD_TOKEN:
                throw InvalidExpression("Invalid character", InvalidExpression::INVALID_CHARACTER, r.getCurrentPosition() - 1, 1);
                break;
            case END:
                // do nothing (just to stop Xcode from complaining)
                break;
        }
        
        if (1 < numOperands)
        {
            throw InvalidExpression("Operand overflow", InvalidExpression::OPERAND_OVERFLOW, 0, expression.length());
        }
        
        tokenType = r.read(token);
    }
    
    if (numOperands != 1)
    {
        throw InvalidExpression("Operand underflow", InvalidExpression::OPERAND_UNDERFLOW, 0, expression.length());
    }
    
    while (!tempStack.empty())
    {
        outputQueue.push(tempStack.top());
        tempStack.pop();
    }
    
    while (!outputQueue.empty())
    {
        if (outputQueue.front() == "(")
            throw InvalidExpression("Expected ')'", InvalidExpression::PARENTHESIS_MISSMATCH, 0, expression.length());
        
        compiledExpression += outputQueue.front() + " ";
        outputQueue.pop();
    }
}

real Expression::evaluate() const
{
    std::istringstream expressionStream(compiledExpression);
    std::stack<real> numberStack;
    std::string token;
    
    while (expressionStream >> token)
    {
        real number;
        if (TokenReader::isName(token))
        {
            if (variables.find(token) != variables.end())
            {
                numberStack.push(variables[token]);
            }
            else
            {
                if (functions.find(token) != functions.end())
                {
                    real arg = numberStack.top();
                    numberStack.pop();
                    
                    numberStack.push(functions[token](arg));
                }
                else
                {
                    std::cout << "Error! No function " << token << " exists." << std::endl;
                }
            }
        }
        else if(TokenReader::isOperator(token))
        {
            if (TokenReader::isUnary(token))
            {
                real firstOperand = numberStack.top();
                numberStack.pop();
                
                numberStack.push(-firstOperand); // There is only one unary operator
            }
            else
            {
                real secondOperand = numberStack.top();
                numberStack.pop();
                real firstOperand = numberStack.top();
                numberStack.pop();
                
                if (token.size() != 1)
                {
                    std::cout << "ERROR! Operator size != 1" << std::endl;
                }
                else
                {
                    switch (token[0])
                    {
                        case '+':
                            numberStack.push(firstOperand + secondOperand);
                            break;
                        case '-':
                            numberStack.push(firstOperand - secondOperand);
                            break;
                        case '*':
                            numberStack.push(firstOperand * secondOperand);
                            break;
                        case '/':
                            numberStack.push(firstOperand / secondOperand);
                            break;
                        case '^':
                            numberStack.push(real_functions::pow(firstOperand, secondOperand));
                            break;
                        default:
                            std::cout << "ERROR! Unkown operator: " << token << std::endl;
                            break;
                    }
                }
            }
        }
        else if (real_functions::parseReal(token, number))
        {
            numberStack.push(number);
        }
        else
        {
            std::cout << "ERROR! Unkown token '" << token << "'" << std::endl;
        }
    }
    
    
    return numberStack.top();
}
