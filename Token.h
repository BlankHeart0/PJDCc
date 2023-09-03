#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum TokenType
{
    //Keyword
    PRINT,

    //Separator
    SEMICOLON, //;

    //Operator
    PLUS, MINUS, STAR, SLASH, // + - * /

    //Constant
    CONSTANT_INT, 

    //Identifier
	ID,

    CODE_EOF
};

static vector<string>TokenType_text
{
    //Keyword
    "PRINT",

    //Separator
    "SEMICOLON", //;

    //Operator
    "PLUS", "MINUS", "STAR", "SLASH", // + - * /

    //Constant
    "CONSTANT_INT", 

    //Identifier
	"ID",

    "CODE_EOF"
};

static unordered_map<string,TokenType>Keyword_map
{
    {"print",PRINT}
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