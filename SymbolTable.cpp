#include "SymbolTable.h"

void VariableTable::Variable_Add(VariableType type,string identifier)
{
    if(!Variable_Exist(identifier))
    {
        table.insert(pair<string,Variable>(identifier,Variable(type,identifier)));
    }
    else VariableTable_Error("add",identifier);
}

bool VariableTable::Variable_Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Variable VariableTable::Variable_Visit(string identifier)
{
    if(!Variable_Exist(identifier))VariableTable_Error("visit",identifier);
    return table[identifier];
}

void VariableTable::VariableTable_Error(string error_message,string identifier)
{
    cout<< "Variable table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}



void FunctionTable::Function_Add(FunctionType type,string identifier,vector<Parameter> parameter_list,int end_lable)
{
    if(!Function_Exist(identifier))
    {
        table.insert(pair<string,Function>(identifier,Function(type,identifier,parameter_list,end_lable)));
    }
    else FunctionTable_Error("add",identifier);
}

bool FunctionTable::Function_Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Function FunctionTable::Function_Visit(string identifier)
{
    if(!Function_Exist(identifier))FunctionTable_Error("visit",identifier);
    return table[identifier];
}

void FunctionTable::FunctionTable_Error(string error_message,string identifier)
{
    cout<< "Function table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}