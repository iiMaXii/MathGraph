//
//  TokenReader.cpp
//  MathGraph
//
//  Created by Max Ekström on 2014-03-01.
//
//

#include "TokenReader.h"

using namespace std;

TokenReader::TokenReader(std::string expression)
: expressionStream(expression)
{}

TokenType TokenReader::read(string &result)
{
    TokenType tokenType = END;
    
    char c = expressionStream.get();
    while (isspace(c))
    { // Ignore any whitespace
        c = expressionStream.get();
    }
    
    if (c != char_traits<char>::eof())
    {
        result = c;
	    
	    switch (c)
	    {
	        case '0':
	        case '1':
	        case '2':
	        case '3':
	        case '4':
	        case '5':
	        case '6':
	        case '7':
	        case '8':
	        case '9':
	        case '.':
	            while (expressionStream.get(c) && (isdigit(c) || c == '.'))
	                result += c;
	            
	            expressionStream.putback(c);
	            
	            tokenType = NUMBER;
	            break;
	        case '+':
	        case '-':
            case '~':
	        case '*':
	        case '/':
	        case '^':
	            tokenType = OPERATOR;
	            break;
	        case '(':
	            tokenType = PARENTHESIS_START;
	            break;
	        case ')':
	            tokenType = PARENTHESIS_END;
	            break;
	        default:
	            if (isalpha(c))
	            {
	                while(expressionStream.get(c) && isalnum(c))
	                    result += c;
	                
	                expressionStream.putback(c);
	                
	                tokenType = NAME;
	            }
	            else
	            {
	                tokenType = BAD_TOKEN;
	            }
                break;
	    }
        
    }
    
    return tokenType;
}

size_t TokenReader::getCurrentPosition()
{
    return expressionStream.tellg();
}

bool TokenReader::isOperator(const string &token)
{
    bool isOpr = false;
    
    if (token.length() == 1)
    {
        switch(token[0])
        {
            case '+':
            case '-':
            case '~': // Unary -
            case '*':
            case '/':
            case '^':
                isOpr = true;
        }
    }
    
    return isOpr;
}

bool TokenReader::isUnary(const std::string &token)
{
    return token == "~";
}

bool TokenReader::isLeftAssociative(const string &token)
{
    return token != "^";
}

int TokenReader::operatorPrecedence(const string &token)
{
    int precedence = -1;
    
    if (token.length() == 1)
    {
        switch (token[0])
        {
            case '+':
            case '-':
                precedence = 2;
                break;
            case '*':
            case '/':
                precedence = 3;
                break;
            case '~':
                precedence = 4;
                break;
            case '^':
                precedence = 5;
                break;
        }
    }
    
    return precedence;
}

bool TokenReader::isName(const string &token)
{
    return !token.empty() && isalpha(token[0]);
}
