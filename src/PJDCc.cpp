#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "CodeGenerator.h"

using namespace std;

char* out_file_path;



enum CompileType
{
    C_EXECUTABLE, C_ASSEMBLY, C_TOKEN, C_AST
};



class PJDCc
{
public:
    void Compile(char* input_file_path,CompileType compile_type);

    string source;
    void Load(char* input_file_path);
    void Load_Test();

    Scanner scanner;
    
    Parser parser;

    CodeGenerator generator;

};



void PJDCc::Load(char* input_file_path)
{
    ifstream file;
    file.open(input_file_path,ios::in);
    if(!file.is_open())
    {
        cout<<"Open the code file error!"<<endl;
        exit(-1);
    }

    source="";
    string buffer;
    while(getline(file,buffer))
    {
        source+=buffer;
        source+='\n';
    }
    
    file.close();
}

void PJDCc::Load_Test()
{
    cout<<source;
}



void PJDCc::Compile(char* input_file_path,CompileType compile_type)
{
    //Read source
    Load(input_file_path);


    //Scan
    scanner.source=this->source;
    scanner.Scan();
    if(compile_type==C_TOKEN)
    {
        scanner.Tokens_PrintTable();
        exit(0);
    }
        

    //Parse
    parser.tokens=scanner.tokens;
    parser.Parse();
    if(compile_type==C_AST)
    {
        parser.ast.AST_Print();
        parser.ast.AST_Clean();
        exit(0);
    }

    //Genernate
    int len=strlen(input_file_path);
    input_file_path[len-1]='s';
    out_file_path=input_file_path;

    generator.ast=parser.ast;
    generator.CodeGenerate();
    if(compile_type==C_ASSEMBLY)
    {
        parser.ast.AST_Clean();
        exit(0);
    }
    
    //Nasm assembly and GCC-ld link
    if(compile_type==C_EXECUTABLE)
    {
        char* nasm_command=new char(14+len);
        sprintf(nasm_command,"%s %s","nasm -f elf64",input_file_path);
        system(nasm_command);

        input_file_path[len-1]='o';
        char* executable_file=new char(len);
        strncpy(executable_file,input_file_path,len);
        executable_file[len-2]='\0';

        char* gcc_command=new char(16+len+(len-2));
        sprintf(gcc_command,"%s %s %s %s","gcc -no-pie",input_file_path,"-o",executable_file);
        system(gcc_command);

        remove(input_file_path);
        input_file_path[len-1]='s';
        remove(input_file_path);

        delete nasm_command;
        delete executable_file;
        delete gcc_command;

        parser.ast.AST_Clean();
        exit(0);
    }
}   

void Print_Usage()
{
    cout<<"Usage: pjdcc [-ESTA] input_file"<<endl;
    cout<<"-E: Generate executable file (default)"<<endl;
    cout<<"-S: Compile to assembly file"<<endl;
    cout<<"-T: Scan the code, print tokens"<<endl;
    cout<<"-A: Parse the code, print abstract syntax tree"<<endl;
    exit(-1);
}

int main(int argc,char* argv[])
{
    PJDCc pjdcc;

    if(argc==2)
    {
        char* input_file_path=argv[1];
        CompileType compile_type=C_EXECUTABLE;

        pjdcc.Compile(input_file_path,compile_type);
    }
    else if(argc==3)
    {
        if(argv[1][0]!='-')Print_Usage();
        if(argv[1][1]!='E'&&argv[1][1]!='S'&&argv[1][1]!='T'&&argv[1][1]!='A')Print_Usage();
        if(strlen(argv[1])!=2)Print_Usage();

        
        char* input_file_path=argv[2];
        CompileType compile_type;

        switch(argv[1][1])
        {
            case 'E':compile_type=C_EXECUTABLE;break;
            case 'S':compile_type=C_ASSEMBLY;break;
            case 'T':compile_type=C_TOKEN;break;
            case 'A':compile_type=C_AST;break;
        }

        pjdcc.Compile(input_file_path,compile_type);
        
    }
    else Print_Usage();

    return 0;
}