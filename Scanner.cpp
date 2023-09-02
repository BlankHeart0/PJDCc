#include <iostream>

#include "Scanner.h"

using namespace std;

void Scanner::Scan()
{
    while(!Is_AtEnd())
    {
        start=current;
        switch(source[current++])
        {
            //one character
            case '+':Add_Token(PLUS);break;
            case '-':Add_Token(MINUS);break;
            case '*':Add_Token(STAR);break;
            case '/':Add_Token(SLASH);break;

            //Blank
            case ' ':case '\r':case '\t':break;
            case '\n':line++;break;

            default:
            //Constant int
                if(Is_Digit(source[current-1]))Scan_Int();
                else Scan_Error("Unexpected character.");
                break;
        }
    }

    //Reach the end
    tokens.push_back(Token(CODE_EOF,"",false,0,line));

    return;
}



void Scanner::Scan_Int()
{
    current--;
    int value=0;
    while(Is_Digit(source[current]))
    {
        value=value*10+source[current]-'0';
        current++;
    }
    Add_Token(CONSTANT_INT,value);
}



void Scanner::Scan_Error(string error_message)
{
    is_error=true;
    cout<<"Scan Error: Line "<<line<<": "<<error_message<<endl;
}



void Scanner::Add_Token(TokenType type,int literal)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,true,literal,line));
}

void Scanner::Add_Token(TokenType type)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,false,0,line));
}



bool Scanner::Is_AtEnd()
{
    return current>=source.size();
}

bool Scanner::Is_Digit(char c)
{
    return c>='0'&&c<='9';
}



void Scanner::Tokens_PrintTable()
{
    cout << "  Line\tType\t\tLexeme\t\tLiteral" << endl<<endl;
	for (Token& token : tokens)
	{
		cout << "  " << token.line << "\t" << TokenType_text[token.type] << "\t";
		if (TokenType_text[token.type].size() < 8)cout << "\t";
		cout<< token.lexeme;
		if (token.have_literal)
		{
			cout << "\t\t";
			switch (token.type)
			{
				case CONSTANT_INT:cout << token.literal; break;
			}
		}
		else cout << "\t\tNULL";
		cout << endl;
	}
}