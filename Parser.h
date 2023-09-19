#pragma once

#include "Token.h"
#include "AbstractSyntaxTree.h"

class Parser
{
public:
    vector<Token>tokens;
    int current;
    AST ast;

    bool DEBUG;

    Parser():current(0),DEBUG(false){}

    void Parse();


// Begin
    ASTNode* Parse_Translation_Unit();


// Definition, Declaration
    ASTNode* Parse_Type();
    ASTNode* Parse_Function_Definition();

    ASTNode* Parse_Variable_Definition();
    ASTNode* Parse_Variable_Declaration();

    ASTNode* Parse_Array_Definition();

// Statement
    ASTNode* Parse_Statement(); 
    
    ASTNode* Parse_Compound_Statement();

    ASTNode* Parse_If_Statement();

    ASTNode* Parse_Iteration_Statement();
    ASTNode* Parse_While_Statement();
    ASTNode* Parse_DoWhile_Statement();

    ASTNode* Parse_Return_Statement();

    ASTNode* Parse_Expression_Statement();


// Expression  
    ASTNode* Parse_Expression();
    ASTNode* Parse_Assignment_Expression();

    ASTNode* Parse_Equality_Expression();
    ASTNode* Parse_Relational_Expression();

    ASTNode* Parse_PlusMinus_Expression();
    ASTNode* Parse_MulDiv_Expression();

    ASTNode* Parse_Unary_Expression();
    ASTNode* Parse_Primary_Expression();

    ASTNode* Parse_FunctionCall_Expression();

    ASTNode* Parse_Address_Expression();
    ASTNode* Parse_Dreference_Expression();
    ASTNode* Parse_Array_Expression();
    
    
//Tools
    bool Match(TokenType expected);
    void Match_Semicolon(ASTNode* root);
    
    bool Peek(TokenType expected);    
    bool Peek(TokenType expected,int n);
    bool Peek_Type();

    Token Previous_Token();

    void Add_Child(ASTNode* root,ASTNode* child);

    bool Is_AtEnd();

    void WhoAmI(string name);
    void Parse_Error(string error_message);
};