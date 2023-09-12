#include "SymbolTable.h"

void VariableTable::Add(Type type,string identifier)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Variable>(identifier,Variable(type,identifier)));
    }
    else Error("add",identifier);
}

bool VariableTable::Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Variable VariableTable::Visit(string identifier)
{
    if(!Exist(identifier))Error("visit",identifier);
    return table[identifier];
}

void VariableTable::Error(string error_message,string identifier)
{
    cout<< "Variable table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}



void FunctionTable::Add(Type type,string identifier,vector<Parameter> parameter_list,int end_lable)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Function>(identifier,Function(type,identifier,parameter_list,end_lable)));
    }
    else Error("add",identifier);
}

bool FunctionTable::Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Function FunctionTable::Visit(string identifier)
{
    if(!Exist(identifier))Error("visit",identifier);
    return table[identifier];
}

void FunctionTable::Error(string error_message,string identifier)
{
    cout<< "Function table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}