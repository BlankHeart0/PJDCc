#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

enum Type
{
    T_VOID,     T_CHAR,         T_INT,          T_LONG,
                T_CHAR_PTR,     T_INT_PTR,      T_LONG_PTR,
                T_CHAR_ARRAY,   T_INT_ARRAY,    T_LONG_ARRAY
};

enum StorePositionType
{
    S_MEMORY,S_STACK
};


class Variable
{
public:
    Type type;
    string identifier;
    StorePositionType position;
    int stack_offset;
    int elem_n;

    Variable(){}
    Variable(Type T,string Identifier,StorePositionType Position,int Elem_N,int Stack_Offset):
        type(T),identifier(Identifier),position(Position),elem_n(Elem_N),stack_offset(Stack_Offset){}
};

class VariableTable
{
public:
    unordered_map<string,Variable>table;

    void Add(Type type,string identifier);
    void Add(Type type,string identifier,int elem_n);
    void Add(Type type,string identifier,int elem_n,int stack_offset);
    
    bool Exist(string identifier);
    Variable& Visit(string identifier);

    void Error(string error_message,string identifier);
};



class Parameter
{
public:
    Type type;
    string identifier;
    
    Parameter(){}
    Parameter(Type T,string Identifier):type(T),identifier(Identifier){}
};

class Function
{
public:
    Type type;
    string identifier;
    vector<Parameter>parameter_list;
    VariableTable local_vartable;
    int end_lable;
    int local_offset;
    int stack_align_offset;

    Function(){}
    Function(Type T,string Identifier,int End_Lable):
        type(T),identifier(Identifier),end_lable(End_Lable),
        local_offset(0),stack_align_offset(0){}
};

class FunctionTable
{
public:
    unordered_map<string,Function>table;

    void Add(Type type,string identifier,int end_lable);
    bool Exist(string identifier);
    Function& Visit(string identifier);

    void Error(string error_message,string identifier);
};

class KissFunctionTable
{
public:
    unordered_set<string>table;

    void Add(string identifier);
    bool Exist(string identifier);

    void Error(string error_message,string identifier);
};