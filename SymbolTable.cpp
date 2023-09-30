#include "SymbolTable.h"

void VariableTable::Add(Type type,string identifier)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Variable>(identifier,Variable(type,identifier,S_MEMORY,1,0)));
    }
    else Error("add",identifier);
}

void VariableTable::Add(Type type,string identifier,int elem_n)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Variable>(identifier,Variable(type,identifier,S_MEMORY,elem_n,0)));
    }
    else Error("add",identifier);
}

void VariableTable::Add(Type type,string identifier,int elem_n,int stack_offset)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Variable>(identifier,Variable(type,identifier,S_STACK,elem_n,stack_offset)));
    }
    else Error("add",identifier);
}



bool VariableTable::Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Variable& VariableTable::Visit(string identifier)
{
    if(!Exist(identifier))Error("visit",identifier);
    return table[identifier];
}

void VariableTable::Error(string error_message,string identifier)
{
    cout<< "Variable table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}



void FunctionTable::Add(Type type,string identifier,int end_lable)
{
    if(!Exist(identifier))
    {
        table.insert(pair<string,Function>(identifier,Function(type,identifier,end_lable)));
    }
    else Error("add",identifier);
}

bool FunctionTable::Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

Function& FunctionTable::Visit(string identifier)
{
    if(!Exist(identifier))Error("visit",identifier);
    return table[identifier];
}

void FunctionTable::Error(string error_message,string identifier)
{
    cout<< "Function table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}



void KissFunctionTable::Add(string identifier)
{
    if(!Exist(identifier))
    {
        table.insert(identifier);
    }
    else Error("add",identifier);
}

bool KissFunctionTable::Exist(string identifier)
{
    if(table.find(identifier)==table.end())return false;
    return true;
}

void KissFunctionTable::Error(string error_message,string identifier)
{
    cout<< "Kiss Function table Error: "<<error_message<<" "<<identifier<<endl;
    exit(4);
}