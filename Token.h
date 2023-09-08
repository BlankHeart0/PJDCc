#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum TokenType
{
    //Keyword
    INT,
    PRINT,
    IF,ELSE,

    //Separator
    SEMICOLON, // ;
    LEFT_PAREN,RIGHT_PAREN, // ( )
    LEFT_BRACE,RIGHT_BRACE, // { }

    //Operator
    PLUS, MINUS, STAR, SLASH, // + - * /
    ASSIGN, // =
    EQUAL, NOT_EQUAL, // == !=
    LESS, LESS_EQUAL, // < <=
    GREATER, GREATER_EQUAL, //> >=
    NOT, // !

    //Constant
    CONSTANT_INT, 

    //Identifier
	ID,

    CODE_EOF
};

static vector<string>TokenType_text
{
    //Keyword
    "INT",
    "PRINT",
    "IF","ELSE",

    //Separator
    "SEMICOLON", //;
    "LEFT_PAREN","RIGHT_PAREN", // ( )
    "LEFT_BRACE","RIGHT_BRACE", // { }

    //Operator
    "PLUS", "MINUS", "STAR", "SLASH", // + - * /
    "ASSIGN", // =
    "EQUAL", "NOT_EQUAL", // == !=
    "LESS", "LESS_EQUAL", // < <=
    "GREATER", "GREATER_EQUAL", //> >=
    "NOT", // !

    //Constant
    "CONSTANT_INT", 

    //Identifier
	"ID",

    "CODE_EOF"
};

static unordered_map<string,TokenType>Keyword_map
{
    {"int",INT},
    {"print",PRINT},
    {"if",IF},{"else",ELSE}
    
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