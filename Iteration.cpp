#include "Iteration.h"

void IterationManger::Enter(IterationType type,int begin_lable,int end_lable)
{
    iteration_stack.push(Iteration(type,begin_lable,-1,end_lable));
}

void IterationManger::Enter(IterationType type,int begin_lable,int intermediate_lable,int end_lable)
{
    iteration_stack.push(Iteration(type,begin_lable,intermediate_lable,end_lable));
}

void IterationManger::Leave()
{
    if(iteration_stack.empty())
    {
        Iteration_Error("Leave iteration failed.");
    }

    iteration_stack.pop();
}



Iteration IterationManger::NowInIteration()
{
    if(iteration_stack.empty())
    {
        Iteration_Error("You are not in an iteration.");
    }

    return iteration_stack.top();
}



void IterationManger::Iteration_Error(string error_message)
{
    cout<<"Iteration Manger Error : "<<error_message<<endl;
    exit(6);
}