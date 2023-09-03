#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "CodeGenerator.h"

using namespace std;


class PJDCc
{
public:
    void Compile(string path);

    string source;
    void Load(string path);
    void Load_Test();

    Scanner S;
    
    Parser P;

    CodeGenerator C;

};


//Read the source code to the string--source
void PJDCc::Load(string path)
{
    //Open
    ifstream file;
    file.open(path,ios::in);
    if(!file.is_open())
    {
        cout<<"Open the code file error!"<<endl;
        return;
    }

    //Load it
    source="";
    string buffer;
    while(getline(file,buffer))
    {
        source+=buffer;
        //Add newline to protect the integrity of the code
        source+='\n';
    }
    
    //Close
    file.close();
}

void PJDCc::Load_Test()
{
    cout<<source;
}

void PJDCc::Compile(string path)
{
    //Read source
    Load(path);
    Load_Test();

    //Scan
    S.source=this->source;
    S.Scan();
    if(!S.is_error)S.Tokens_PrintTable();
    else return;

    //Parse
    P.tokens=S.tokens;
    P.Parse();
    if(!P.is_error)cout<<"--- Parse Successfully! ---"<<endl;
    else return;

    //Genernate
    C.ast=P.ast;
    C.CodeGenerate("Calculator.asm");
}


int main()
{
    PJDCc pjdcc;
    pjdcc.Compile("./test/CodeGen_Test.c");

    return 0;
}