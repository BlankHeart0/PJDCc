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



class RegisterManager
{
public:
    vector<Register>Register_Table;

    RegisterManager();

//Alloc and Free
    int Alloc();
    void Free(int register_i);
    void FreeAll();

//Visit
    string Name(int register_i);
    string Name(int register_i,int byte);
    
    void Register_Error(string error_message);
};