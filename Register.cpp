#include "Register.h"

GeneralRegister::GeneralRegister()
{   
    spill_register=0;

    table.push_back(Register(vector<string>{"r10","r10d","r10w","r10b"}));
    table.push_back(Register(vector<string>{"r11","r11d","r11w","r11b"}));
    table.push_back(Register(vector<string>{"r12","r12d","r12w","r12b"}));
    table.push_back(Register(vector<string>{"r13","r13d","r13w","r13b"}));
    table.push_back(Register(vector<string>{"r14","r14d","r14w","r14b"}));
    table.push_back(Register(vector<string>{"r15","r15d","r15w","r15b"}));

    FreeAll();
}



int GeneralRegister::Alloc()
{
    int Register_i=0;
    for(;Register_i<table.size();Register_i++)
    {
        if(table[Register_i].free)
        {
            table[Register_i].free=false;
            break;
        }
    }

    if(Register_i>=table.size())
    {
        Register_i=spill_register%table.size();
        spill_register++;

        Append("\tpush\t"+Name(Register_i));
    }
    return Register_i;
}

void GeneralRegister::Free(int register_i)
{
    if(table[register_i].free)
    {
        Register_Error("Can not free a general regisgter.");
    }

    if(spill_register>0)
    {
        spill_register--;

        Append("\tpop\t"+Name(register_i));
    }
    else table[register_i].free=true;
}

void GeneralRegister::FreeAll()
{
    for(int register_i=0;register_i<table.size();register_i++)
    {
        table[register_i].free=true;
    }
}



string GeneralRegister::Name(int register_i)
{
    return Name(register_i,8);
    
}

string GeneralRegister::Name(int register_i,int byte)
{
    string name;

    if(register_i>=0&&register_i<table.size())
    {
        switch(byte)
        {
            case 8:
                name=table[register_i].name[0];break;
            case 4:
                name=table[register_i].name[1];break;
            case 2:
                name=table[register_i].name[2];break;
            case 1:
                name=table[register_i].name[3];break;
        }
    }
    else Register_Error("Can not visit this general register.");
    
    return name;
}



void GeneralRegister::FlashUsed()
{
    vector<string>new_used_register;
    for(int i=0;i<table.size();i++)
    {
        if(!table[i].free)
        {
            new_used_register.push_back(Name(i));
        }
    }

    used_register=new_used_register;
}



 void GeneralRegister::Append(string assembly_code)
 {
    OutFile.open(out_file_path,ios::app);
    if(!OutFile.is_open())Register_Error("Append the assembly file error!");
    
    OutFile<<assembly_code<<endl;

    OutFile.close();
 }





ParameterRegister::ParameterRegister()
{
    table.push_back(Register(vector<string>{"rdi","edi","di","dil"}));
    table.push_back(Register(vector<string>{"rsi","esi","si","sil"}));
    table.push_back(Register(vector<string>{"rdx","edx","dx","dl"}));
    table.push_back(Register(vector<string>{"rcx","ecx","cx","cl"}));
    table.push_back(Register(vector<string>{"r8","r8d","r8w","r8b"}));
    table.push_back(Register(vector<string>{"r9","r9d","r9w","r9b"}));

    FreeAll();
}



void ParameterRegister::Alloc(int register_i)
{
    if(table[register_i].free)
    {
        table[register_i].free=false;
    }
    else Register_Error("Can not alloc a parameter regisgter.");
}

void ParameterRegister::Free(int register_i)
{
    if(!table[register_i].free)
    {
        table[register_i].free=true;
    }
    else Register_Error("Can not free a parameter regisgter.");
}

void ParameterRegister::FreeAll()
{
    for(int register_i=0;register_i<table.size();register_i++)
    {
        table[register_i].free=true;
    }
}



string ParameterRegister::Name(int register_i)
{
    return Name(register_i,8);
}

string ParameterRegister::Name(int register_i,int byte)
{
    string name;

    if(register_i>=0&&register_i<table.size())
    {   
        switch(byte)
        {
            case 8:
                name=table[register_i].name[0];break;
            case 4:
                name=table[register_i].name[1];break;
            case 2:
                name=table[register_i].name[2];break;
            case 1:
                name=table[register_i].name[3];break;
        }
    }
    else Register_Error("Can not visit this parameter register.");

    return name;
}



void ParameterRegister::FlashUsed()
{
    vector<string>new_used_register;
    for(int i=0;i<table.size();i++)
    {
        if(!table[i].free)
        {
            new_used_register.push_back(Name(i));
        }
    }

    used_register=new_used_register;
}



static void Register_Error(string error_message)
{
    cout<< "Register Manager Error : "<<error_message<<endl;
    exit(3);
}
