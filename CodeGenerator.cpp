#include "CodeGenerator.h"

void CodeGenerator::CodeGenerate(string path)
{
    OutFile.open(path,ios::out);
    if(!OutFile.is_open())
    {
        cout<<"Create the assembly file error!"<<endl;
        return;
    }

    cout<<endl<<"--- CodeGenerate Begin ---"<<endl;

    CodeGenerate_Head();
    CodeGenerate_Translation_Unit(ast.Translation);
    CodeGenerate_Tail();

    cout<<"--- CodeGenerate Done ---"<<endl<<endl;

    OutFile.close();
}

void CodeGenerator::CodeGenerate_Head()
{
    OutFile<<"section .text"<<endl;
    OutFile<<"\tglobal main"<<endl;
    OutFile<<"\textern printf"<<endl<<endl;
    OutFile<<"main:"<<endl;
}



void CodeGenerator::CodeGenerate_Translation_Unit(ASTNode* root)
{
    WhoAmI("CodeGenerate_Translation_Unit");

    for(int i=0;i<root->Children.size();i++)
    {
        CodeGenerate_Statement(root->Children[i]);
    }
}



void CodeGenerator::CodeGenerate_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Statement");
    int print_ri=CodeGenerate_Expression(root->Children[1]);
    Print(print_ri);
}



int CodeGenerator::CodeGenerate_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression");

    return CodeGenerate_PlusMinus_Expression(FirstChild(root));
}

int CodeGenerator::CodeGenerate_PlusMinus_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_PlusMinus_Expression");

    int result_ri=CodeGenerate_MulDiv_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_MulDiv_Expression(root->Children[i+1]);
        if(root->Children[i]->type==AST_PLUS)result_ri=Add(result_ri,temp_ri);
        else if(root->Children[i]->type==AST_MINUS)result_ri=Sub(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_MulDiv_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_MulDiv_Expression");

    int result_ri=CodeGenerate_Unary_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Unary_Expression(root->Children[i+1]);
        if(root->Children[i]->type==AST_STAR)result_ri=Mul(result_ri,temp_ri);
        else if(root->Children[i]->type==AST_SLASH)result_ri=Div(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_Unary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Unary_Expression");

    return CodeGenerate_Primary_Expression(FirstChild(root));
}
    
int CodeGenerator::CodeGenerate_Primary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Primary_Expression");

    return Load(FirstChild(root)->value);
}



void CodeGenerator::CodeGenerate_Tail()
{
    OutFile<<endl<<"exit:"<<endl;
    OutFile<<"\tmov\trax, 0"<<endl;
    OutFile<<"\tret"<<endl<<endl;

    OutFile<<"section .data"<<endl;
    OutFile<<"\tformat: db \"%d\",0XA,0"<<endl;
}



ASTNode* CodeGenerator::FirstChild(ASTNode* root)
{
    if(root->Children.size()>=1)return root->Children[0];
    return nullptr;
}



int CodeGenerator::Load(int value)
{
    int register_i=R.Alloc();

    OutFile<<"\tmov\t"<<R.Register_Table[register_i].name<<", "<<value<<endl;
    return register_i;
}

int CodeGenerator::Add(int r1_i,int r2_i)
{
    OutFile<<"\tadd\t"<<R.Register_Table[r1_i].name<<", "<<R.Register_Table[r2_i].name<<endl;
    
    R.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Sub(int r1_i,int r2_i)
{
    OutFile<<"\tsub\t"<<R.Register_Table[r1_i].name<<", "<<R.Register_Table[r2_i].name<<endl;
    
    R.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Mul(int r1_i,int r2_i)
{
    OutFile<<"\timul\t"<<R.Register_Table[r1_i].name<<", "<<R.Register_Table[r2_i].name<<endl;

    R.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Div(int r1_i,int r2_i)
{
    OutFile<<"\tmov\trax, "<<R.Register_Table[r1_i].name<<endl;
    OutFile<<"\tcqo"<<endl;
    OutFile<<"\tidiv\t"<<R.Register_Table[r2_i].name<<endl;
    OutFile<<"\tmov\t"<<R.Register_Table[r1_i].name<<", rax"<<endl;

    R.Free(r2_i);
    return r1_i;
}

void CodeGenerator::Print(int r_i)
{
    OutFile<<"\tpush\trbp"<<endl;

    OutFile<<"\tmov\trdi, format"<<endl;
    OutFile<<"\tmov\trsi, "<<R.Name(r_i)<<endl;
    OutFile<<"\tmov\trax, 0"<<endl;
    OutFile<<"\tcall\tprintf"<<endl;

    OutFile<<"\tpop\trbp"<<endl;
    R.Free(r_i);
}

void CodeGenerator::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}