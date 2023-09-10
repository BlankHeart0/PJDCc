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

    Parser():current(0),DEBUG(true){}

    void Parse();

    ASTNode* Translation_Unit();

    ASTNode* Function_Definition();
    ASTNode* Function_Type();

    ASTNode* Statement();
    ASTNode* Compound_Statement();

    ASTNode* Print_Statement();

    ASTNode* Variable_Definition();
    ASTNode* Variable_Declaration();
    ASTNode* Variable_Type();

    ASTNode* If_Statement();

    ASTNode* Iteration_Statement();
    ASTNode* While_Statement();
    ASTNode* DoWhile_Statement();

    ASTNode* Expression_Statement();

    ASTNode* Return_Statement();


    ASTNode* Expression();
    ASTNode* Assignment_Expression();
    ASTNode* Equality_Expression();
    ASTNode* Relational_Expression();
    ASTNode* PlusMinus_Expression();
    ASTNode* MulDiv_Expression();
    ASTNode* Unary_Expression();
    ASTNode* Primary_Expression();
    ASTNode* FunctionCall_Expression();

    void Parse_Error(string error_message);

    bool Is_AtEnd();
    bool Match(TokenType expected);
    bool Peek(TokenType expected);    
    bool Peek(TokenType expected,int n);
    bool Peek_VariableType();

    Token Previous_Token();
    void Add_Child(ASTNode* root,ASTNode* child);
    void Match_Semicolon(ASTNode* root);

    void WhoAmI(string name);
};