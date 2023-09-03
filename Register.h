#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Register
{
public:
    string name;
    bool free;
    
    Register(string Name,bool Free):name(Name),free(Free){}
};

class RegisterController
{
public:
    vector<Register>Register_Table;

    RegisterController();

    int Alloc();
    void Free(int register_i);
    void FreeAll();

    string Name(int register_i);
    void RegisterController_Error(string error_message);
};