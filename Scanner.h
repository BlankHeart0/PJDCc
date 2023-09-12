#pragma once

#include <vector>
#include "Token.h"

class Scanner
{
public:

    string source;
    vector<Token>tokens;

    int start;
    int current;
    int line;
    

    Scanner():start(0),current(0),line(1){}

    void Scan();

    void Scan_Int();
    void Scan_IdentifierKeyword();

//Tools
    bool Match(char expected);

    void Add_Token(TokenType type,int literal);
    void Add_Token(TokenType type);

    bool Is_AtEnd();
    bool Is_Digit(char c);
    bool Is_AlphaUnderline(char c);
    bool Is_DigitAlphaUnderline(char c);
    
    
    void Scan_Error(string error_message);

    void Tokens_PrintTable();
};
