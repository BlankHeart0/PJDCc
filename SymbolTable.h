#pragma once

#include <iostream>
#include <unordered_set>

using namespace std;

class SymbolTable
{
public:
    unordered_set<string>table;

    void Symbol_Add(string symbol);

    bool Symbol_Exist(string symbol);

};
