#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

enum VariableType
{
    V_INT,V_CHAR,V_LONG
};

class Variable
{
public:
    VariableType type;
    string identifier;
    
    Variable(){}
    Variable(VariableType Type,string Identifier):type(Type),identifier(Identifier){}
};

class VariableTable
{
public:
    unordered_map<string,Variable>table;

    void Variable_Add(VariableType type,string identifier);
    bool Variable_Exist(string identifier);
    Variable Variable_Visit(string identifier);

    void VariableTable_Error(string error_message,string identifier);
};


enum FunctionType
{
    F_VOID,F_INT,F_CHAR,F_LONG
};

class Parameter
{
public:
    VariableType type;
    string identifier;
};

class Function
{
public:
    FunctionType type;
    string identifier;
    vector<Parameter>parameter_list;
    int end_lable;
    
    Function(){}
    Function(FunctionType Type,string Identifier,vector<Parameter> Parameter_List,int End_Lable):
        type(Type),identifier(Identifier),parameter_list(Parameter_List),end_lable(End_Lable){}
};

class FunctionTable
{
public:
    unordered_map<string,Function>table;

    void Function_Add(FunctionType type,string identifier,vector<Parameter> parameter_list,int end_lable);
    bool Function_Exist(string identifier);
    Function Function_Visit(string identifier);

    void FunctionTable_Error(string error_message,string identifier);
};

class SymbolTable
{
public:
    VariableTable variable_table;
    FunctionTable function_table;
};