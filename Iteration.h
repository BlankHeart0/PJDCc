#pragma once

#include<iostream>
#include<stack>

using namespace std;

enum IterationType
{
    I_WHILE,I_DOWHILE,I_FOR
};

class Iteration
{
public:
    IterationType type;

    int begin_lable;
    int intermediate_lable;
    int end_lable;

    Iteration(IterationType Type,int Begin_Lable,int Intermediate_Lable,int End_Lable):
        type(Type),begin_lable(Begin_Lable),intermediate_lable(Intermediate_Lable),end_lable(End_Lable){}

};


class IterationManger
{
public:
    stack<Iteration>iteration_stack;

    void Enter(IterationType type,int begin_lable,int end_lable);
    void Enter(IterationType type,int begin_lable,int intermediate_lable,int end_lable);
    void Leave();

    Iteration NowInIteration();

    void Iteration_Error(string error_message);
};