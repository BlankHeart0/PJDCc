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
    void Scan_Char();
	void Scan_String();
	
    void Scan_IdentifierKeyword();

//Tools
    bool Match(char expected);

    char NextChar();

    void Add_Token(TokenType type);
    void Add_Token(TokenType type,int literal_int);
    void Add_Token(TokenType type,char literal_char);
    void Add_Token(TokenType type,string literal_string);

    bool Is_AtEnd();
    bool Is_Digit(char c);
    bool Is_AlphaUnderline(char c);
    bool Is_DigitAlphaUnderline(char c);
    
    
    void Scan_Error(string error_message);

    void Tokens_PrintTable();
};
