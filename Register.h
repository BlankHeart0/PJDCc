#pragma once

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

extern char* out_file_path;

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
    int spill_register;
    vector<Register> table;
    vector<string> used_register;

    ofstream OutFile;

    GeneralRegister();

    int Alloc();
    void Free(int register_i);
    void FreeAll();

    string Name(int register_i);
    string Name(int register_i,int byte);

    void FlashUsed();

    void Append(string assembly_code);
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