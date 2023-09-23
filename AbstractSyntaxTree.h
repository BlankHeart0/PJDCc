#pragma once

#include <vector>
#include <iostream>

#include "Token.h"

using namespace std;

enum ASTNodeType
{
//Terminal

    //Keyword
    AST_VOID, AST_CHAR, AST_INT, AST_LONG,
    AST_PRINT,
    AST_IF, AST_ELSE,
    AST_WHILE, AST_DO,
    AST_RETURN,

    //Separator
    AST_SEMICOLON, AST_COMMA, // ; ,
    AST_LEFT_PAREN, AST_RIGHT_PAREN, // ( )
    AST_LEFT_SQUARE,AST_RIGHT_SQUARE, // [ ]
    AST_LEFT_BARCE, AST_RIGHT_BRACE, // { }

    //Operator
    AST_PLUS, AST_MINUS, AST_STAR, AST_SLASH, AST_MOD, // + - * / %
    AST_INC, AST_DEC, // ++ --
    AST_ASSIGN, // =

    AST_LEFT_SHIFT, AST_RIGHT_SHIFT, // << >>

    AST_EQUAL, AST_NOT_EQUAL, // == !=
    AST_LESS, AST_LESS_EQUAL, // < <=
    AST_GREATER, AST_GREATER_EQUAL, // > >=
    AST_NOT, AST_AND, AST_OR,// ! && ||

    AST_INVERT, AST_AMPERSAND, AST_BITOR, AST_XOR, // ~ & | ^

    //Constant
    AST_CONSTANT_INT, AST_CONSTANT_CHAR, AST_CONSTANT_STRING,


    //Identifier
	AST_ID,


//Nonterminal

    // Begin
    TRANSLATION_UNIT,


    // Definition, Declaration
    TYPE,
    FUNCTION_DEFINITION,
    PARAMETER_LIST,

    GLOBALVARIABLE_DEFINITION,
    GLOBALARRAY_DEFINITION,

    LOCALVARIABLE_DEFINITION,
    LOCALVARIABLE_DECLARATION,
    
    

    // Statement
    STATEMENT,
        
    COMPOUND_STATEMENT,
 
    IF_STATEMENT,

    ITERATION_STATEMENT,
    WHILE_STATEMENT,
    DOWHILE_STATEMENT,

    RETURN_STATEMENT,

    EXPRESSION_STATEMENT,

   
    // Expression  
    EXPRESSION,
    ASSIGNMENT_EXPRESSION,

    LOGICOR_EXPRESSION,
    LOGICAND_EXPRESSION,

    OR_EXPRESSION,
    XOR_EXPRESSION,
    AND_EXPRESSION,

    EQUALITY_EXPRESSION,
    RELATIONAL_EXPRESSION,

    SHIFT_EXPRESSION,

    PLUSMINUS_EXPRESSION,
    MULDIV_EXPRESSION,

    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION,

    FUNCTIONCALL_EXPRESSION,

    ADDRESS_EXPRESSION,
    DREFERENCE_EXPRESSION, 
    ARRAY_EXPRESSION,

    INCDECPREFIX_EXPRESSION,
    INCDECPOSTFIX_EXPRESSION
};

static vector<string>ASTNodeType_text
{
//Terminal

    //Keyword
    "AST_VOID", "AST_CHAR", "AST_INT", "AST_LONG",
    "AST_PRINT",
    "AST_IF", "AST_ELSE",
    "AST_WHILE", "AST_DO",
    "AST_RETURN",

    //Separator
    "AST_SEMICOLON", "AST_COMMA", // ; ,
    "AST_LEFT_PAREN", "AST_RIGHT_PAREN", // ( )
    "AST_LEFT_SQUARE","AST_RIGHT_SQUARE", // [ ]
    "AST_LEFT_BARCE", "AST_RIGHT_BRACE", // { }

    //Operator
    "AST_PLUS", "AST_MINUS", "AST_STAR", "AST_SLASH", "AST_MOD", // + - * / %
    "AST_INC", "AST_DEC", // ++ --
    "AST_ASSIGN", // =

    "AST_LEFT_SHIFT", "AST_RIGHT_SHIFT", // << >>

    "AST_EQUAL", "AST_NOT_EQUAL", // == !=
    "AST_LESS", "AST_LESS_EQUAL", // < <=
    "AST_GREATER", "AST_GREATER_EQUAL", // > >=
    "AST_NOT", "AST_AND", "AST_OR",// ! && ||

    "AST_INVERT", "AST_AMPERSAND", "AST_BITOR", "AST_XOR", // ~ & | ^

    //Constant
    "AST_CONSTANT_INT", "AST_CONSTANT_CHAR", "AST_CONSTANT_STRING",

    //Identifier
	"AST_ID",


//Nonterminal

    // Begin
    "TRANSLATION_UNIT",


    // Definition, Declaration
    "TYPE",
    "FUNCTION_DEFINITION",
    "PARAMETER_LIST",
    
    "GLOBALVARIABLE_DEFINITION",
    "GLOBALARRAY_DEFINITION",

    "LOCALVARIABLE_DEFINITION",
    "LOCALVARIABLE_DECLARATION",



    // Statement
    "STATEMENT",
        
    "COMPOUND_STATEMENT",
 
    "IF_STATEMENT",

    "ITERATION_STATEMENT",
    "WHILE_STATEMENT",
    "DOWHILE_STATEMENT",

    "RETURN_STATEMENT",

    "EXPRESSION_STATEMENT",

   
    // Expression  
    "EXPRESSION",
    "ASSIGNMENT_EXPRESSION",

    "LOGICOR_EXPRESSION",
    "LOGICAND_EXPRESSION",

    "OR_EXPRESSION",
    "XOR_EXPRESSION",
    "AND_EXPRESSION",

    "EQUALITY_EXPRESSION",
    "RELATIONAL_EXPRESSION",

    "SHIFT_EXPRESSION",

    "PLUSMINUS_EXPRESSION",
    "MULDIV_EXPRESSION",

    "UNARY_EXPRESSION",
    "PRIMARY_EXPRESSION",
    
    "FUNCTIONCALL_EXPRESSION",

    "ADDRESS_EXPRESSION",
    "DREFERENCE_EXPRESSION",  
    "ARRAY_EXPRESSION",

    "INCDECPREFIX_EXPRESSION",
    "INCDECPOSTFIX_EXPRESSION"
};



class ASTNode
{
public:
    ASTNodeType type;
    string lexeme;
    int line;

    bool is_terminal;

    bool literal_have;
    LiteralType literal_type;
    int literal_int;
    char literal_char;
    string literal_string;
    
    vector<ASTNode*>Children;

    ASTNode(ASTNodeType Type):
        type(Type),lexeme(""),is_terminal(false){}

    ASTNode(ASTNodeType Type,Token Token):
        type(Type),lexeme(Token.lexeme),is_terminal(true),line(Token.line)

        ,literal_have(Token.literal_have),literal_type(Token.literal_type),
        literal_int(Token.literal_int),literal_char(Token.literal_char),literal_string(Token.literal_string){}

};


class AST
{
public:
    ASTNode* Translation;

    AST():Translation(nullptr){}

    void AST_Print(ASTNode* root,int depth);
    void AST_Print();
};