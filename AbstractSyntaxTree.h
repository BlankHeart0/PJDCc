#pragma once

#include <vector>
#include <iostream>
using namespace std;

enum ASTNodeType
{
    //Terminal

    //Keyword
    AST_PRINT,

    //Separator
    AST_SEMICOLON, //;

    //Operator
    AST_PLUS, AST_MINUS, AST_STAR, AST_SLASH, // + - * /

    //Constant
    AST_CONSTANT_INT,


    //Nonterminal

    TRANSLATION_UNIT,
    STATEMENT,
    EXPRESSION,
    PLUSMINUS_EXPRESSION,MULDIV_EXPRESSION,UNARY_EXPRESSION,PRIMARY_EXPRESSION 
};

static vector<string>ASTNodeType_text
{
    //Terminal

    //Keyword
    "AST_PRINT",

    //Separator
    "AST_SEMICOLON", //;
    
    //Operator
    "AST_PLUS", "AST_MINUS", "AST_STAR", "AST_SLASH", // + - * /

    //Constant
    "AST_CONSTANT_INT",

    //Nonterminal

    "TRANSLATION_UNIT",
    "STATEMENT",
    "EXPRESSION",
    "PLUSMINUS_EXPRESSION","MULDIV_EXPRESSION","UNARY_EXPRESSION","PRIMARY_EXPRESSION" 
};



class ASTNode
{
public:
    bool is_terminal;
    ASTNodeType type;
    int value;

    vector<ASTNode*>Children;

    ASTNode(bool Is_Terminal,ASTNodeType Type,int Value):
        is_terminal(Is_Terminal),type(Type),value(Value){}

};

class AST
{
public:
    ASTNode* Translation;

    AST():Translation(nullptr){}

    void AST_Print(ASTNode* root,int depth);
    void AST_Print();
};