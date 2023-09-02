#pragma once

#include <vector>
#include "Token.h"

class Scanner
{
public:
    string source;
    int start;
    int current;
    int line;
    vector<Token>tokens;

    bool is_error;

    Scanner():start(0),current(0),line(1),is_error(false){}

    void Scan();

    void Scan_Int();

    void Scan_Error(string error_message);

    void Add_Token(TokenType type,int literal);
    void Add_Token(TokenType type);

    bool Is_AtEnd();
    bool Is_Digit(char c);

    void Tokens_PrintTable();
};
