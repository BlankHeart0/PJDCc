#pragma once

#include <fstream>

#include "Register.h"
#include "SymbolTable.h"
#include "AbstractSyntaxTree.h"

class CodeGenerator
{
public:
    AST ast;
    RegisterController RC;
    SymbolTable ST;
    ofstream OutFile;

    bool is_error;
    bool DEBUG;

    CodeGenerator():is_error(false),DEBUG(true){}

    void CodeGenerate(string path);
    void CodeGenerate_Head();

    void CodeGenerate_Translation_Unit(ASTNode* root);

    void CodeGenerate_Statement(ASTNode* root);
    void CodeGenerate_Print_Statement(ASTNode* root);
    void CodeGenerate_Assignment_Statement(ASTNode* root);

    void CodeGenerate_Variable_Definition(ASTNode* root);
    string CodeGenerate_Variable_Declaration(ASTNode* root);

    int CodeGenerate_Expression(ASTNode* root);
    int CodeGenerate_PlusMinus_Expression(ASTNode* root);
    int CodeGenerate_MulDiv_Expression(ASTNode* root);
    int CodeGenerate_Unary_Expression(ASTNode* root);
    int CodeGenerate_Primary_Expression(ASTNode* root);

    void CodeGenerate_Tail();

    ASTNode* FirstChild(ASTNode* root);

    int Load(int value);
    int Load(string identifier);
    void Store(int r_i,string identifier);
    void CreateVar(string identifier);

    int Add(int r1_i,int r2_i);
    int Sub(int r1_i,int r2_i);
    int Mul(int r1_i,int r2_i);
    int Div(int r1_i,int r2_i);
    void Print(int r_i);

    void CodeGenerate_Error(string error_message);
    void CodeGenerate_Error(string error_message,ASTNode* root);

    void WhoAmI(string name);
};