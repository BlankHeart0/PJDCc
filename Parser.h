#pragma once

#include "Token.h"
#include "AbstractSyntaxTree.h"

class Parser
{
public:
    vector<Token>tokens;
    int current;
    AST ast;

    bool is_error;
    bool DEBUG;

    Parser():current(0),is_error(false),DEBUG(false){}

    void Parse();

    ASTNode* Translation_Unit();

    ASTNode* Statement();

    ASTNode* Print_Statement();
    ASTNode* Assignment_Statement();

    ASTNode* Variable_Definition();
    ASTNode* Variable_Declaration();

    ASTNode* Expression();
    ASTNode* PlusMinus_Expression();
    ASTNode* MulDiv_Expression();
    ASTNode* Unary_Expression();
    ASTNode* Primary_Expression();

    void Parse_Error(string error_message);

    bool Is_AtEnd();
    bool Match(TokenType expected);
    bool Peek(TokenType expected);

    Token Previous_Token();

    void WhoAmI(string name);
};