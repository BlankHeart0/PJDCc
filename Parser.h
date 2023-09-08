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

    ASTNode* Translation_Unit();

    ASTNode* Statement();
    ASTNode* Compound_Statement();

    ASTNode* Print_Statement();
    ASTNode* Assignment_Statement();

    ASTNode* Variable_Definition();
    ASTNode* Variable_Declaration();

    ASTNode* If_Statement();

    ASTNode* Iteration_Statement();
    ASTNode* While_Statement();
    ASTNode* DoWhile_Statement();


    ASTNode* Expression();
    ASTNode* Equality_Expression();
    ASTNode* Relational_Expression();
    ASTNode* PlusMinus_Expression();
    ASTNode* MulDiv_Expression();
    ASTNode* Unary_Expression();
    ASTNode* Primary_Expression();

    void Parse_Error(string error_message);

    bool Is_AtEnd();
    bool Match(TokenType expected);
    bool Peek(TokenType expected);

    Token Previous_Token();
    void Add_Child(ASTNode* root,ASTNode* child);
    void Match_Semicolon(ASTNode* root);

    void WhoAmI(string name);
};