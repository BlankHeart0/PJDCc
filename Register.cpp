#include "Register.h"

RegisterManager::RegisterManager()
{
    General_Register_Table.push_back(Register("r10",true));
    General_Register_Table.push_back(Register("r11",true));
    General_Register_Table.push_back(Register("r12",true));
    General_Register_Table.push_back(Register("r13",true));

    FreeAll();

    Parameter_Register_Table=
    {
        vector<string>{"rdi","edi","di","dil"},
        vector<string>{"rsi","esi","si","sil"},
        vector<string>{"rdx","edx","dx","dl"},
        vector<string>{"rcx","ecx","cx","cl"},
        vector<string>{"r8","r8d","r8w","r8b"},
        vector<string>{"r9","r9d","r9w","r9b"}
    };
    
}



//Alloc and Free
int RegisterManager::Alloc()
{
    int Register_i=0;
    for(;Register_i<General_Register_Table.size();Register_i++)
    {
        if(General_Register_Table[Register_i].free)
        {
            General_Register_Table[Register_i].free=false;
            break;
        }
    }

    if(Register_i>=General_Register_Table.size())
    {
        Register_Error("Can not alloc a regisgter.");
    }

    return Register_i;
}

void RegisterManager::Free(int register_i)
{
    if(General_Register_Table[register_i].free)
    {
        Register_Error("Can not free a regisgter.");
    }

    General_Register_Table[register_i].free=true;
}

void RegisterManager::FreeAll()
{
    for(int register_i=0;register_i<General_Register_Table.size();register_i++)
    {
        General_Register_Table[register_i].free=true;
    }
}



//Visit
string RegisterManager::Name(int register_i)
{
    return Name(register_i,8);
}

string RegisterManager::Name(int register_i,int byte)
{
    string name;

    if(register_i>=0&&register_i<General_Register_Table.size())
    {
        switch(byte)
        {
            case 8:
                name=General_Register_Table[register_i].name;break;
            case 4:
                name=General_Register_Table[register_i].name+"d";break;
            case 2:
                name=General_Register_Table[register_i].name+"w";break;
            case 1:
                name=General_Register_Table[register_i].name+"b";break;
        }
    }
    else Register_Error("Can not visit this register.");
    
    return name;
}



string RegisterManager::Parameter_Register_Name(int register_i)
{
    return Parameter_Register_Name(register_i,8);
}

string RegisterManager::Parameter_Register_Name(int register_i,int byte)
{
    string name;

    if(register_i>=0&&register_i<Parameter_Register_Table.size())
    {   
        switch(byte)
        {
            case 8:
                name=Parameter_Register_Table[register_i][0];break;
            case 4:
                name=Parameter_Register_Table[register_i][1];break;
            case 2:
                name=Parameter_Register_Table[register_i][2];break;
            case 1:
                name=Parameter_Register_Table[register_i][3];break;
        }
    }
    else Register_Error("Can not visit this register.");

    return name;
}



void RegisterManager::Register_Error(string error_message)
{
    cout<< "Register Controller Error : "<<error_message<<endl;
    exit(3);
}
