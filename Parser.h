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


// Begin
    ASTNode* Parse_Translation_Unit();


// Definition, Declaration
    ASTNode* Parse_Kiss_Declaration();

    ASTNode* Parse_Type();
    ASTNode* Parse_Function_Definition();
    ASTNode* Parse_Parameter_List();

    ASTNode* Parse_GlobalVariable_Definition(); 
    ASTNode* Parse_GlobalArray_Definition();
    ASTNode* Parse_Initialize_List();

    ASTNode* Parse_LocalVariable_Definition();   
    ASTNode* Parse_LocalVariable_Declaration();

   
// Statement
    ASTNode* Parse_Statement(); 
    
    ASTNode* Parse_Compound_Statement();

    ASTNode* Parse_If_Statement();

    ASTNode* Parse_Iteration_Statement();
    ASTNode* Parse_While_Statement();
    ASTNode* Parse_DoWhile_Statement();
    ASTNode* Parse_For_Statement();

    ASTNode* Parse_Continue_Statement();
    ASTNode* Parse_Break_Statement();
    
    ASTNode* Parse_Return_Statement();

    ASTNode* Parse_Expression_Statement();


// Expression  
    ASTNode* Parse_Expression();
    ASTNode* Parse_Assignment_Expression();

    ASTNode* Parse_LogicOr_Expression();
    ASTNode* Parse_LogicAnd_Expression();

    ASTNode* Parse_Or_Expression();
    ASTNode* Parse_Xor_Expression();
    ASTNode* Parse_And_Expression();

    ASTNode* Parse_Equality_Expression();
    ASTNode* Parse_Relational_Expression();

    ASTNode* Parse_Shift_Expression();

    ASTNode* Parse_PlusMinus_Expression();
    ASTNode* Parse_MulDiv_Expression();

    ASTNode* Parse_Unary_Expression();
    ASTNode* Parse_Primary_Expression();

    ASTNode* Parse_FunctionCall_Expression();

    ASTNode* Parse_Address_Expression();
    ASTNode* Parse_Dreference_Expression();
    ASTNode* Parse_Array_Expression();
    
    ASTNode* Parse_Sizeof_Expression();
    
    ASTNode* Parse_IncDecPrefix_Expression();
    ASTNode* Parse_IncDecPostfix_Expression();
    
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