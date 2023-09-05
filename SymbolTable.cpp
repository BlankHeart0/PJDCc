#include "SymbolTable.h"

void SymbolTable::Symbol_Add(string symbol)
{
    if(!Symbol_Exist(symbol))
    {
        table.insert(symbol);
    }
}

bool SymbolTable::Symbol_Exist(string symbol)
{
    if(table.find(symbol)==table.end())return false;
    return true;
}