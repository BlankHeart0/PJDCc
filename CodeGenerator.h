#pragma once

#include <fstream>

#include "AbstractSyntaxTree.h"
#include "Register.h"
#include "SymbolTable.h"


class CodeGenerator
{
public:
    AST ast;

    RegisterManager register_manager;
    VariableTable variable_table;
    FunctionTable function_table;

    ofstream OutFile;

    int LableNumber;
    int StringNumber;
    string NowInFunction;

    string HeadInstruction;
    string TailData;

    bool DEBUG;

    CodeGenerator():LableNumber(0),StringNumber(0),NowInFunction(""),TailData(""),DEBUG(false){}

    void CodeGenerate(string path);

    void CodeGenerate_Head();
    void CodeGenerate_Tail();


// Begin
    void CodeGenerate_Translation_Unit(ASTNode* root);


// Definition, Declaration    
    Type CodeGenerate_Type(ASTNode* root);
    void CodeGenerate_Function_Definition(ASTNode* root);

    void CodeGenerate_Variable_Definition(ASTNode* root);
    string CodeGenerate_Variable_Declaration(ASTNode* root);

    void CodeGenerate_Array_Definition(ASTNode* root);

// Statement
    void CodeGenerate_Statement(ASTNode* root);
    
    void CodeGenerate_Compound_Statement(ASTNode* root);

    void CodeGenerate_If_Statement(ASTNode* root);

    void CodeGenerate_Iteration_Statement(ASTNode* root);
    void CodeGenerate_While_Statement(ASTNode* root);
    void CodeGenerate_DoWhile_Statement(ASTNode* root);

    void CodeGenerate_Return_Statement(ASTNode* root);
    
    void CodeGenerate_Expression_Statement(ASTNode* root);

    
// Expression  
    int CodeGenerate_Expression(ASTNode* root);
    int CodeGenerate_Assignment_Expression(ASTNode* root);

    int CodeGenerate_LogicOr_Expression(ASTNode* root);
    int CodeGenerate_LogicAnd_Expression(ASTNode* root);

    int CodeGenerate_Or_Expression(ASTNode* root);
    int CodeGenerate_Xor_Expression(ASTNode* root);
    int CodeGenerate_And_Expression(ASTNode* root);

    int CodeGenerate_Equality_Expression(ASTNode* root);
    int CodeGenerate_Relational_Expression(ASTNode* root);

    int CodeGenerate_Shift_Expression(ASTNode* root);

    int CodeGenerate_PlusMinus_Expression(ASTNode* root);
    int CodeGenerate_MulDiv_Expression(ASTNode* root);

    int CodeGenerate_Unary_Expression(ASTNode* root);
    int CodeGenerate_Primary_Expression(ASTNode* root);

    int CodeGenerate_FunctionCall_Expression(ASTNode* root);

    int CodeGenerate_Address_Expression(ASTNode* root);
    int CodeGenerate_Dreference_Expression(ASTNode* root);
    int CodeGenerate_Array_Expression(ASTNode* root);

    int CodeGenerate_IncDecPrefix_Expression(ASTNode* root);
    int CodeGenerate_IncDecPostfix_Expression(ASTNode* root);

    

//Atomic instruction
    int Load(int value);
    int Load(string identifier);

    void Store(int r_i,string identifier,bool free);
    void Store(int r1_i,int r2_i,Type type,bool free);

    void CreateVar(string identifier);
    void CreateVar(string identifier,int size);

    int CreateString(string literal_string);
    string StringToIntlist(string liter_string);
    int NewStringNubmer();

    int Address(string identifier);
    int Dereference(int r_i,Type ptr_type);

    int Comma(int r1_i,int r2_i);

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
    
    int Add(int r1_i,int r2_i);
    int Sub(int r1_i,int r2_i);
    int Mul(int r1_i,int r2_i);
    int Div(int r1_i,int r2_i);
    int Mod(int r1_i,int r2_i);

    int And(int r1_i,int r2_i);
    int Or(int r1_i,int r2_i);
    int Xor(int r1_i,int r2_i);
    int Invert(int r_i);
    int Not(int r_i);
    int Negate(int r_i);

    int ShiftLeftConstant(int r1_i,int constant);
    int ShiftLeft(int r1_i,int r2_i);
    int ShiftRight(int r1_i,int r2_i);

    int Inc(string identifier,string pre_post);
    int Dec(string identifier,string pre_post);

    void FunctionHead(string identifier);
    void FunctionTail(string identifier);
    int FunctionCall(int r_i,string identifier);
    void Return(int r_i,string identifier);



//Tools
    Type Type_To_PtrType(Type type);
    Type Type_To_ArrayType(Type type);

    int Address_ScaleFactor(Type type);
    int Dreference_ScaleFactor(Type ptr_type);

    ASTNode* FirstChild(ASTNode* root);

    void WhoAmI(string name);    

    void CodeGenerate_Error(string error_message);
    void CodeGenerate_Error(string error_message,ASTNode* root);
};