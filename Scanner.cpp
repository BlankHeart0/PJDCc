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
            case '[':Add_Token(LEFT_SQUARE);break;
            case ']':Add_Token(RIGHT_SQUARE);break;
            case '{':Add_Token(LEFT_BRACE);break;
            case '}':Add_Token(RIGHT_BRACE);break;
            case ',':Add_Token(COMMA);break;
            case ';':Add_Token(SEMICOLON);break;

            case '+':Add_Token(Match('+')?INC:PLUS);break;
            case '-':Add_Token(Match('-')?DEC:MINUS);break;
            case '*':Add_Token(STAR);break;
            case '%':Add_Token(MOD);break;

            case '~':Add_Token(INVERT);break;
            case '^':Add_Token(XOR);break;
            
            //two character
            case '=':Add_Token(Match('=')?EQUAL:ASSIGN);break;
            case '!':Add_Token(Match('=')?NOT_EQUAL:NOT);break;
            case '<':Add_Token(Match('=')?LESS_EQUAL:   Match('<')?LEFT_SHIFT:  LESS);break;
            case '>':Add_Token(Match('=')?GREATER_EQUAL:Match('>')?RIGHT_SHIFT: GREATER);break;
            
            case '&':Add_Token(Match('&')?AND:AMPERSAND);break;
            case '|':Add_Token(Match('|')?OR:BITOR);break;

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

			//Constant char
			case '\'':Scan_Char(); break;

			//Constant string
			case '"':Scan_String(); break;

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
    tokens.push_back(Token(CODE_EOF,"",line,false));

    return;
}



void Scanner::Scan_Int()
{
    current--;
    int value_int=0;

    while(!Is_AtEnd()&&Is_Digit(source[current]))
    {
        value_int=value_int*10+source[current]-'0';
        current++;
    }

    Add_Token(CONSTANT_INT,value_int);
}

void Scanner::Scan_Char()
{
    char value_char=NextChar();

    if(Match('\''))Add_Token(CONSTANT_CHAR,value_char);
	else Scan_Error("Incomplete char.");
}

void Scanner::Scan_String()
{
    string value_string;

    while (!Is_AtEnd() &&source[current]!='\n'&&source[current] != '"')
    {
        value_string.push_back(NextChar());
    }

	if (Match('"')) Add_Token(CONSTANT_STRING,value_string);
	else Scan_Error("Incomplete string.");
}



void Scanner::Scan_IdentifierKeyword()
{
    while(!Is_AtEnd()&&Is_DigitAlphaUnderline(source[current]))current++;
    string text=source.substr(start,current-start);
    auto it=Keyword_map.find(text);
    if(it==Keyword_map.end())Add_Token(ID);
    else Add_Token(it->second);
}



//Tools
bool Scanner::Match(char expected)
{
    if(Is_AtEnd()||source[current]!=expected)return false;
    current++;
    return true;
}



char Scanner::NextChar()
{
    if(Match('\\'))
    {
        if(Match('a'))return'\a';
        else if(Match('b'))return'\b';
        else if(Match('f'))return'\f';
        else if(Match('n'))return'\n';
        else if(Match('r'))return'\t';
        else if(Match('t'))return'\t';
        else if(Match('v'))return'\v';
        else if(Match('\\'))return'\\';
        else if(Match('\''))return'\'';
        else if(Match('"'))return'"';
        else if(Match('0'))return'\0';

        else Scan_Error("Wrong escape character.");
    }

    //Ordinary character
    return source[current++];
}



void Scanner::Add_Token(TokenType type)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,line,false));
}

void Scanner::Add_Token(TokenType type,int literal_int)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,line,true,L_INT,literal_int));
}

void Scanner::Add_Token(TokenType type,char literal_char)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,line,true,L_CHAR,literal_char));
}

void Scanner::Add_Token(TokenType type,string literal_string)
{
    string lexeme=source.substr(start,current-start);
    tokens.push_back(Token(type,lexeme,line,true,L_STRING,literal_string));
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



void Scanner::Scan_Error(string error_message)
{
    cout<<"Scan Error: Line "<<line<<": "<<error_message<<endl;
    exit(1);
}



void Scanner::Tokens_PrintTable()
{
    cout << "  Line\tType\t\tLexeme\t\tLiteral" << endl<<endl;
	for (Token& token : tokens)
	{
		cout << "  " << token.line << "\t" << TokenType_text[token.type] << "\t";
		if (TokenType_text[token.type].size() < 8)cout << "\t";
		cout<< token.lexeme;
		if (token.literal_have)
		{
			cout << "\t\t";
			switch (token.type)
			{
				case CONSTANT_INT:cout << token.literal_int; break;
                case CONSTANT_CHAR:cout << token.literal_char; break;
				case CONSTANT_STRING:cout << token.literal_string; break;
			}
		}
		else cout << "\t\tNULL";
		cout << endl;
	}
}
