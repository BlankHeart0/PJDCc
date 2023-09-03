#pragma once

#include <fstream>

#include "Register.h"
#include "AbstractSyntaxTree.h"

class CodeGenerator
{
public:
    AST ast;
    RegisterController R;
    ofstream OutFile;

    bool DEBUG;

    CodeGenerator():DEBUG(true){}

    void CodeGenerate(string path);
    void CodeGenerate_Head();

    void CodeGenerate_Translation_Unit(ASTNode* root);

    void CodeGenerate_Statement(ASTNode* root);

    int CodeGenerate_Expression(ASTNode* root);
    int CodeGenerate_PlusMinus_Expression(ASTNode* root);
    int CodeGenerate_MulDiv_Expression(ASTNode* root);
    int CodeGenerate_Unary_Expression(ASTNode* root);
    int CodeGenerate_Primary_Expression(ASTNode* root);

    void CodeGenerate_Tail();

    ASTNode* FirstChild(ASTNode* root);

    int Load(int value);
    int Add(int r1_i,int r2_i);
    int Sub(int r1_i,int r2_i);
    int Mul(int r1_i,int r2_i);
    int Div(int r1_i,int r2_i);
    void Print(int r_i);

    void WhoAmI(string name);
};