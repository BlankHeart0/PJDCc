#include <iostream>

#include "Token.h"
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
            case '(':Add_Token(LEFT_PAREN);break;
            case ')':Add_Token(RIGHT_PAREN);break;
            case '{':Add_Token(LEFT_BRACE);break;
            case '}':Add_Token(RIGHT_BRACE);break;
            
            case ';':Add_Token(SEMICOLON);break;
            case '+':Add_Token(PLUS);break;
            case '-':Add_Token(MINUS);break;
            case '*':Add_Token(STAR);break;

            //two character
            case '=':Add_Token(Match('=')?EQUAL:ASSIGN);break;
            case '!':Add_Token(Match('=')?NOT_EQUAL:NOT);break;
            case '<':Add_Token(Match('=')?LESS_EQUAL:LESS);break;
            case '>':Add_Token(Match('=')?GREATER_EQUAL:GREATER);break;
            
            //Annotation
            case '/':
                if(Match('/'))while(!Is_AtEnd()&&source[current]!='\n')current++;
                else if(Match('*'))
                {
                    while(!Is_AtEnd()&&(source[current]!='/'||source[current-1]!='*'))
                    {
                        if(source[current]=='\n')line++;
                        current++;
                    }
                    if(Is_AtEnd())Scan_Error("Incomplete annotation.");
                    else current++;
                }
                else Add_Token(SLASH);
                break;

            //Blank
            case ' ':case '\r':case '\t':break;
            case '\n':line++;break;

            default:
            //Constant int
                if(Is_Digit(source[current-1]))Scan_Int();
				//Identifier and Keyword
                else if(Is_AlphaUnderline(source[current-1]))Scan_IdentifierKeyword();
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
    while(!Is_AtEnd()&&Is_Digit(source[current]))
    {
        value=value*10+source[current]-'0';
        current++;
    }
    Add_Token(CONSTANT_INT,value);
}

void Scanner::Scan_IdentifierKeyword()
{
    while(!Is_AtEnd()&&Is_DigitAlphaUnderline(source[current]))current++;
    string text=source.substr(start,current-start);
    auto it=Keyword_map.find(text);
    if(it==Keyword_map.end())Add_Token(ID);
    else Add_Token(it->second);
}


void Scanner::Scan_Error(string error_message)
{
    cout<<"Scan Error: Line "<<line<<": "<<error_message<<endl;
    exit(1);
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

bool Scanner::Is_AlphaUnderline(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')||c=='_';
}

bool Scanner::Is_DigitAlphaUnderline(char c)
{
	return Is_Digit(c) || Is_AlphaUnderline(c);
}

bool Scanner::Match(char expected)
{
    if(Is_AtEnd()||source[current]!=expected)return false;
    current++;
    return true;
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