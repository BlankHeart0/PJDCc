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

    Scanner():start(0),current(0),line(1){}

    void Scan();

    void Scan_Int();
    void Scan_IdentifierKeyword();

    void Scan_Error(string error_message);

    void Add_Token(TokenType type,int literal);
    void Add_Token(TokenType type);

    bool Is_AtEnd();
    bool Is_Digit(char c);
    bool Is_AlphaUnderline(char c);
    bool Is_DigitAlphaUnderline(char c);
    
    bool Match(char expected);
    

    void Tokens_PrintTable();
};
