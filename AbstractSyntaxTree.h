#pragma once

#include <vector>
#include <iostream>

#include "Token.h"

using namespace std;

enum ASTNodeType
{
    //Terminal

    //Keyword
    AST_INT,
    AST_PRINT,
    AST_IF,AST_ELSE,
    AST_WHILE,AST_DO,

    //Separator
    AST_SEMICOLON, //;
    AST_LEFT_PAREN,AST_RIGHT_PAREN, // ( )
    AST_LEFT_BARCE,AST_RIGHT_BRACE, // { }

    //Operator
    AST_PLUS, AST_MINUS, AST_STAR, AST_SLASH, // + - * /
    AST_ASSIGN, // =
    AST_EQUAL, AST_NOT_EQUAL, // == !=
    AST_LESS, AST_LESS_EQUAL, // < <=
    AST_GREATER, AST_GREATER_EQUAL, //> >=
    AST_NOT, // !

    //Constant
    AST_CONSTANT_INT,

    //Identifier
	AST_ID,


    //Nonterminal

    TRANSLATION_UNIT,

    STATEMENT,
    COMPOUND_STATEMENT,
 
    PRINT_STATEMENT,
    ASSIGNMENT_STATEMENT,

    VARIABLE_DEFINITION,
    VARIABLE_DECLARATION,

    IF_STATEMENT,

    ITERATION_STATEMENT,
    WHILE_STATEMENT,
    DOWHILE_STATEMENT,

    EXPRESSION,
    EQUALITY_EXPRESSION,
    RELATIONAL_EXPRESSION,

    PLUSMINUS_EXPRESSION,
    MULDIV_EXPRESSION,
    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION 
};

static vector<string>ASTNodeType_text
{
    //Terminal

    //Keyword
    "AST_INT",
    "AST_PRINT",
    "AST_IF","AST_ELSE",
    "AST_WHILE","AST_DO",

    //Separator
    "AST_SEMICOLON", //;
    "AST_LEFT_PAREN","AST_RIGHT_PAREN", // ( )
    "AST_LEFT_BARCE","AST_RIGHT_BRACE", // { }

    //Operator
    "AST_PLUS", "AST_MINUS", "AST_STAR", "AST_SLASH", // + - * /
    "AST_ASSIGN", // =
    "AST_EQUAL", "AST_NOT_EQUAL", // == !=
    "AST_LESS", "AST_LESS_EQUAL", // < <=
    "AST_GREATER", "AST_GREATER_EQUAL", //> >=
    "AST_NOT", // !

    //Constant
    "AST_CONSTANT_INT",

    //Identifier
	"AST_ID",


    //Nonterminal

    "TRANSLATION_UNIT",

    "STATEMENT",
    "COMPOUND_STATEMENT",
    
    "PRINT_STATEMENT",
    "ASSIGNMENT_STATEMENT",

    "VARIABLE_DEFINITION",
    "VARIABLE_DECLARATION",

    "IF_STATEMENT",
    
    "ITERATION_STATEMENT",
    "WHILE_STATEMENT",
    "DOWHILE_STATEMENT",

    "EXPRESSION",
    "EQUALITY_EXPRESSION",
    "RELATIONAL_EXPRESSION",
    
    "PLUSMINUS_EXPRESSION",
    "MULDIV_EXPRESSION",
    "UNARY_EXPRESSION",
    "PRIMARY_EXPRESSION" 
};



class ASTNode
{
public:
    ASTNodeType type;
    string lexeme;
    bool is_terminal;
    int literal;
    int line;

    vector<ASTNode*>Children;

    ASTNode(ASTNodeType Type):
        type(Type),lexeme(""),is_terminal(false),literal(0),line(-1){}

    ASTNode(ASTNodeType Type,Token Token):
        type(Type),lexeme(Token.lexeme),is_terminal(true),literal(Token.literal),line(Token.line){}

};

class AST
{
public:
    ASTNode* Translation;

    AST():Translation(nullptr){}

    void AST_Print(ASTNode* root,int depth);
    void AST_Print();
};