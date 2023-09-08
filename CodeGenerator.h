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
    int LableNumber;

    bool DEBUG;

    CodeGenerator():LableNumber(0),DEBUG(false){}

    void CodeGenerate(string path);
    void CodeGenerate_Head();

    void CodeGenerate_Translation_Unit(ASTNode* root);

    void CodeGenerate_Statement(ASTNode* root);
    void CodeGenerate_Compound_Statement(ASTNode* root);

    void CodeGenerate_Print_Statement(ASTNode* root);
    void CodeGenerate_Assignment_Statement(ASTNode* root);

    void CodeGenerate_Variable_Definition(ASTNode* root);
    string CodeGenerate_Variable_Declaration(ASTNode* root);

    void CodeGenerate_If_Statement(ASTNode* root);


    int CodeGenerate_Expression(ASTNode* root);
    int CodeGenerate_Equality_Expression(ASTNode* root);
    int CodeGenerate_Relational_Expression(ASTNode* root);
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

    int Compare(int r1_i,int r2_i,string setx);
    int Equal(int r1_i,int r2_i);
    int NotEqual(int r1_i,int r2_i);
    int Less(int r1_i,int r2_i); 
    int LessEqual(int r1_i,int r2_i);
    int Greater(int r1_i,int r2_i);
    int GreaterEqual(int r1_i,int r2_i);
    void CompareZero(int r_i); 

    int NewLable();
    void LablePrint(int lable_number);
    void Jump(string jump,int lable_numbr);

    void CodeGenerate_Error(string error_message);
    void CodeGenerate_Error(string error_message,ASTNode* root);

    void WhoAmI(string name);
};