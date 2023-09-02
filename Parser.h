#pragma once

#include "Token.h"
#include "AbstractSyntaxTree.h"

class Parser
{
public:
    vector<Token>tokens;
    int current;
    AST A;

    bool is_error;
    bool DEBUG;

    Parser():current(0),is_error(false),DEBUG(false){}

    void Parse();

    ASTNode* Translation_Unit();

    ASTNode* Expression();
    ASTNode* PlusMinus_Expression();
    ASTNode* MulDiv_Expression();
    ASTNode* Unary_Expression();
    ASTNode* Primary_Expression();

    void Parse_Error(string error_message);

    bool Is_AtEnd();
    bool Match(TokenType expected);
    TokenType Previous_TokenType();

    void WhoAmI(string name);
};