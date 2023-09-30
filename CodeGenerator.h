#pragma once

#include <fstream>

#include "AbstractSyntaxTree.h"
#include "Register.h"
#include "Iteration.h"
#include "SymbolTable.h"


class CodeGenerator
{
public:
    AST ast;

    RegisterManager register_manager;
    IterationManger iteration_manger;

    VariableTable global_vartable;
    FunctionTable function_table;
    KissFunctionTable kissfunction_table;

    ofstream OutFile;

    int LableNumber;
    int StringNumber;
    string NowInFunction;
    
    //string HeadData;
    string TailData;

    bool DEBUG;

    CodeGenerator():LableNumber(0),StringNumber(0),NowInFunction(""),TailData(""),DEBUG(false){}

    void CodeGenerate(string path);

    void CodeGenerate_Head();
    void CodeGenerate_Tail();


// Begin
    void CodeGenerate_Translation_Unit(ASTNode* root);


// Definition, Declaration
    void CodeGenerate_Kiss_Declaration(ASTNode* root);

    Type CodeGenerate_Type(ASTNode* root);
    void CodeGenerate_Function_Definition(ASTNode* root);
    void CodeGenerate_Parameter_List(ASTNode* root);
    
    void CodeGenerate_GlobalVariable_Definition(ASTNode* root);
    void CodeGenerate_GlobalArray_Definition(ASTNode* root);
    vector<int> CodeGenerate_Initialize_List(ASTNode* root);

    void CodeGenerate_LocalVariable_Definition(ASTNode* root);
    string CodeGenerate_LocalVariable_Declaration(ASTNode* root,bool is_parameter);
    

// Statement
    void CodeGenerate_Statement(ASTNode* root);
    
    void CodeGenerate_Compound_Statement(ASTNode* root);

    void CodeGenerate_If_Statement(ASTNode* root);

    void CodeGenerate_Iteration_Statement(ASTNode* root);
    void CodeGenerate_While_Statement(ASTNode* root);
    void CodeGenerate_DoWhile_Statement(ASTNode* root);
    void CodeGenerate_For_Statement(ASTNode* root);

    void CodeGenerate_Continue_Statement(ASTNode* root);
    void CodeGenerate_Break_Statement(ASTNode* root);

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

    int CodeGenerate_Sizeof_Expression(ASTNode* root);

    int CodeGenerate_IncDecPrefix_Expression(ASTNode* root);
    int CodeGenerate_IncDecPostfix_Expression(ASTNode* root);

    

//Atomic instruction
    int Load(int constant_value);
    int LoadGlobalVar(string identifier);
    int LoadLocalVar(string identifier);

    void StoreGlobalVar(int r_i,string identifier,bool free);
    void StoreLocalVar(int r_i,string identifier,bool free);
    void Store(int r1_i,int r2_i,Type type,bool free);

    void CreateGlobalVar(string identifier,int initialization_value);
    void CreateGlobalArray(string identifier,vector<int> initialize_list);
    int CreateLocalVar(Type type,string identifier,bool is_parameter);

    int CreateString(string literal_string);
    string StringToIntlist(string liter_string);
    int NewString();

    int AddressGlobalVar(string identifier);
    int AddressLocalVar(string identifier);

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

    int IncGlobalVar(string identifier,string pre_post);
    int IncLocalVar(string identifier,string pre_post);
    
    int DecGlobalVar(string identifier,string pre_post);
    int DecLocalVar(string identifier,string pre_post);

    void FunctionHead(string identifier);
    void FunctionTail(string identifier);
    int FunctionCall(string identifier);
    void Return(int r_i,string identifier);
    
    void LoadParameter();
    void StoreArgument(ASTNode* expression_node);



//Tools
    void Guarantee_Exist_GlobalVartable(string identifier,ASTNode* error_node);
    void Guarantee_InExist_GlobalVartable(string identifier,ASTNode* error_node);
    void Guarantee_Exist_LocalVartable(string identifier,ASTNode* error_node);
    void Guarantee_InExist_LocalVartable(string identifier,ASTNode* error_node);

    VariableTable& Local_Vartable();
    Function& Now_Function();

    Type Type_To_PtrType(Type type);
    Type Type_To_ArrayType(Type type);

    int Address_ScaleFactor(Type type);
    int Dreference_ScaleFactor(Type ptr_type);

    void Calculate_StackOffset(ASTNode* compound_node);

    ASTNode* FirstChild(ASTNode* root);

    void WhoAmI(string name);    

    void CodeGenerate_Error(string error_message);
    void CodeGenerate_Error(string error_message,ASTNode* error_node);
};