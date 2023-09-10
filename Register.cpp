#include "Register.h"

RegisterController::RegisterController()
{
    Register_Table.push_back(Register("r8",true));
    Register_Table.push_back(Register("r9",true));
    Register_Table.push_back(Register("r10",true));
    Register_Table.push_back(Register("r11",true));
    
    //Register_Table.push_back(Register("r12",true));
    //Register_Table.push_back(Register("r13",true));
}

int RegisterController::Alloc()
{
    int Register_i=0;
    for(;Register_i<Register_Table.size();Register_i++)
    {
        if(Register_Table[Register_i].free)
        {
            Register_Table[Register_i].free=false;
            break;
        }
    }

    if(Register_i>=Register_Table.size())
    {
        RegisterController_Error("Can not alloc a regisgter.");
        exit(1);
    }

    return Register_i;
}

void RegisterController::Free(int register_i)
{
    if(Register_Table[register_i].free)
    {
        RegisterController_Error("Can not free a regisgter.");
        exit(1);
    }

    Register_Table[register_i].free=true;
}

void RegisterController::FreeAll()
{
    for(int register_i=0;register_i<Register_Table.size();register_i++)
    {
        Register_Table[register_i].free=true;
    }
}


string RegisterController::Name(int register_i)
{
    return Name(register_i,8);
}

string RegisterController::Name(int register_i,int byte)
{
    string name;

    if(register_i>=0&&register_i<Register_Table.size())
    {
        switch(byte)
        {
            case 8:
                name=Register_Table[register_i].name;break;
            case 4:
                name=Register_Table[register_i].name+"d";break;
            case 2:
                name=Register_Table[register_i].name+"w";break;
            case 1:
                name=Register_Table[register_i].name+"b";break;
        }
    }
    else RegisterController_Error("Can not visit this register.");
    
    return name;
}


void RegisterController::RegisterController_Error(string error_message)
{
    cout<< "Register Controller Error : "<<error_message<<endl;
    exit(3);
}
