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
    S.Tokens_PrintTable();

    //Parse
    P.tokens=S.tokens;
    P.Parse();
    cout<<"--- Parse Successfully! ---"<<endl;

    //Genernate
    C.ast=P.ast;
    C.CodeGenerate("./test/CodeGen_Test/If.asm");
    
    cout<<endl<<"--- Compile Successfully! ---"<<endl;

}   


int main()
{
    PJDCc pjdcc;

    //pjdcc.Compile("./test/Scanner_Test.c");
    //pjdcc.Compile("./test/Parser_Test.c");


    //CodeGen
    //pjdcc.Compile("./test/CodeGen_Test/test.c");

    //pjdcc.Compile("./test/CodeGen_Test/Calculator.c");
    //pjdcc.Compile("./test/CodeGen_Test/Variable.c");
    //pjdcc.Compile("./test/CodeGen_Test/Compare.c");
    pjdcc.Compile("./test/CodeGen_Test/If.c");

    return 0;
}