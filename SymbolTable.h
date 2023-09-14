#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

enum Type
{
    T_VOID,     T_CHAR,     T_INT,     T_LONG,
                T_CHAR_PTR, T_INT_PTR, T_LONG_PTR
};



class Variable
{
public:
    Type type;
    string identifier;
    
    Variable(){}
    Variable(Type T,string Identifier):type(T),identifier(Identifier){}
};

class VariableTable
{
public:
    unordered_map<string,Variable>table;

    void Add(Type type,string identifier);
    bool Exist(string identifier);
    Variable Visit(string identifier);

    void Error(string error_message,string identifier);
};



class Parameter
{
public:
    Type type;
    string identifier;
};

class Function
{
public:
    Type type;
    string identifier;
    vector<Parameter>parameter_list;
    int end_lable;
    
    Function(){}
    Function(Type T,string Identifier,vector<Parameter> Parameter_List,int End_Lable):
        type(T),identifier(Identifier),parameter_list(Parameter_List),end_lable(End_Lable){}
};

class FunctionTable
{
public:
    unordered_map<string,Function>table;

    void Add(Type type,string identifier,vector<Parameter> parameter_list,int end_lable);
    bool Exist(string identifier);
    Function Visit(string identifier);

    void Error(string error_message,string identifier);
};