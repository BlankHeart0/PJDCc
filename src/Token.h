#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum TokenType
{
    //Keyword
    KISS,
    VOID, CHAR, INT, LONG,
    IF, ELSE,
    WHILE, DO, FOR,
    CONTINUE,BREAK,
    RETURN,
    SIZEOF,

    //Separator
    SEMICOLON, COMMA, // ; ,
    LEFT_PAREN, RIGHT_PAREN, // ( )
    LEFT_SQUARE,RIGHT_SQUARE, // [ ]
    LEFT_BRACE, RIGHT_BRACE, // { }

    //Operator
    PLUS, MINUS, STAR, SLASH, MOD, // + - * / %
    INC, DEC, // ++ --
    ASSIGN, // =

    LEFT_SHIFT, RIGHT_SHIFT, // << >>

    EQUAL, NOT_EQUAL, // == !=
    LESS, LESS_EQUAL, // < <=
    GREATER, GREATER_EQUAL, // > >=
    NOT, AND, OR,// ! && ||

    INVERT, AMPERSAND, BITOR, XOR, // ~ & | ^

    COLON, QUESTION, // : ?

    //Constant
    CONSTANT_INT, CONSTANT_CHAR, CONSTANT_STRING,

    //Identifier
	ID,

    CODE_EOF
};

static vector<string>TokenType_text
{
    //Keyword
    "KISS",
    "VOID", "CHAR", "INT", "LONG",
    "IF", "ELSE",
    "WHILE", "DO", "FOR",
    "CONTINUE","BREAK",
    "RETURN",
    "SIZEOF",

    //Separator
    "SEMICOLON", "COMMA", // ; ,
    "LEFT_PAREN", "RIGHT_PAREN", // ( )
    "LEFT_SQUARE","RIGHT_SQUARE", // [ ]
    "LEFT_BRACE", "RIGHT_BRACE", // { }

    //Operator
    "PLUS", "MINUS", "STAR", "SLASH", "MOD", // + - * / %
    "INC", "DEC", // ++ --
    "ASSIGN", // =

    "LEFT_SHIFT", "RIGHT_SHIFT", // << >>

    "EQUAL", "NOT_EQUAL", // == !=
    "LESS", "LESS_EQUAL", // < <=
    "GREATER", "GREATER_EQUAL", // > >=
    "NOT", "AND", "OR",// ! && ||

    "INVERT", "AMPERSAND", "BITOR", "XOR", // ~ & | ^

    "COLON", "QUESTION", // : ?

    //Constant
    "CONSTANT_INT", "CONSTANT_CHAR", "CONSTANT_STRING",

    //Identifier
	"ID",

    "CODE_EOF"
};

static unordered_map<string,TokenType>Keyword_map
{
    {"kiss",KISS},
    {"void",VOID},{"char",CHAR},{"int",INT},{"long",LONG},
    {"if",IF},{"else",ELSE},
    {"while",WHILE},{"do",DO},{"for",FOR},
    {"continue",CONTINUE},{"break",BREAK},
    {"return",RETURN},
    {"sizeof",SIZEOF}
};

enum LiteralType
{
    L_INT,L_CHAR,L_STRING
};

class Token
{
public:
    TokenType type;
    string lexeme;
    int line;

    bool literal_have;
    LiteralType literal_type;
    
    int literal_int;
    char literal_char;
    string literal_string;
    
    Token(TokenType Type,string Lexeme,int Line,bool Lhave)
        :type(Type),lexeme(Lexeme),line(Line),literal_have(Lhave){}

    Token(TokenType Type,string Lexeme,int Line,bool Lhave,LiteralType Ltype,int Lint)
        :type(Type),lexeme(Lexeme),line(Line),
        literal_have(Lhave),literal_type(Ltype),literal_int(Lint){}
    
    Token(TokenType Type,string Lexeme,int Line,bool Lhave,LiteralType Ltype,char Lchar)
        :type(Type),lexeme(Lexeme),line(Line),
        literal_have(Lhave),literal_type(Ltype),literal_char(Lchar){}

    Token(TokenType Type,string Lexeme,int Line,bool Lhave,LiteralType Ltype,string Lstring)
        :type(Type),lexeme(Lexeme),line(Line),
        literal_have(Lhave),literal_type(Ltype),literal_string(Lstring){}
};