#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum TokenType
{
    //Keyword
    VOID, CHAR, INT, LONG,
    PRINT,
    IF, ELSE,
    WHILE, DO,
    RETURN,

    //Separator
    SEMICOLON, COMMA, // ; ,
    LEFT_PAREN, RIGHT_PAREN, // ( )
    LEFT_SQUARE,RIGHT_SQUARE, // [ ]
    LEFT_BRACE, RIGHT_BRACE, // { }

    //Operator
    PLUS, MINUS, STAR, SLASH, // + - * /
    ASSIGN, // =

    EQUAL, NOT_EQUAL, // == !=
    LESS, LESS_EQUAL, // < <=
    GREATER, GREATER_EQUAL, // > >=
    NOT, AND, OR,// ! && ||

    AMPERSAND, // &

    //Constant
    CONSTANT_INT, 

    //Identifier
	ID,

    CODE_EOF
};

static vector<string>TokenType_text
{
    //Keyword
    "VOID", "CHAR", "INT", "LONG",
    "PRINT",
    "IF", "ELSE",
    "WHILE", "DO",
    "RETURN",

    //Separator
    "SEMICOLON", "COMMA", // ; ,
    "LEFT_PAREN", "RIGHT_PAREN", // ( )
    "LEFT_SQUARE","RIGHT_SQUARE", // [ ]
    "LEFT_BRACE", "RIGHT_BRACE", // { }

    //Operator
    "PLUS", "MINUS", "STAR", "SLASH", // + - * /
    "ASSIGN", // =
    
    "EQUAL", "NOT_EQUAL", // == !=
    "LESS", "LESS_EQUAL", // < <=
    "GREATER", "GREATER_EQUAL", // > >=
    "NOT", "AND", "OR",// ! && ||

    "AMPERSAND", // &

    //Constant
    "CONSTANT_INT", 

    //Identifier
	"ID",

    "CODE_EOF"
};

static unordered_map<string,TokenType>Keyword_map
{
    {"void",VOID},{"char",CHAR},{"int",INT},{"long",LONG},
    {"print",PRINT},
    {"if",IF},{"else",ELSE},
    {"while",WHILE},{"do",DO},
    {"return",RETURN}
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