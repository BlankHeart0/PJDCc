#pragma once

#include <iostream>
#include <vector>

using namespace std;

enum TokenType
{
    //Operator
    PLUS, MINUS, STAR, SLASH, // + - * /

    //Constant
    CONSTANT_INT, 

    CODE_EOF
};

static vector<string>TokenType_text
{
    //Operator
    "PLUS", "MINUS", "STAR", "SLASH", // + - * /

    //Constant
    "CONSTANT_INT", 

    "CODE_EOF"
};

class Token
{
public:
    TokenType type;
    string lexeme;
    bool have_literal;
    int literal;
    int line;

    Token(TokenType Type,string Lexeme,bool Have_Literal,int Literal,int Line)
        :type(Type),lexeme(Lexeme),have_literal(Have_Literal),literal(Literal),line(Line){}
};