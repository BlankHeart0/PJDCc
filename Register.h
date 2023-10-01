#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Register
{
public:
    vector<string> name;
    bool free;
    
    Register(vector<string> Name):name(Name),free(true){}
};



class GeneralRegister
{
public:
    vector<Register> table;
    vector<string> used_register;

    GeneralRegister();

    int Alloc();
    void Free(int register_i);
    void FreeAll();

    string Name(int register_i);
    string Name(int register_i,int byte);

    void FlashUsed();
};

class ParameterRegister
{
public:
    vector<Register> table;
    vector<string> used_register;

    ParameterRegister();

    void Alloc(int register_i);
    void Free(int register_i);
    void FreeAll();

    string Name(int register_i);
    string Name(int register_i,int byte); 

    void FlashUsed();
};

static void Register_Error(string error_message);