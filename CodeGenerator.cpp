#include "CodeGenerator.h"

void CodeGenerator::CodeGenerate(string path)
{
    OutFile.open(path,ios::out);
    if(!OutFile.is_open())
    {
        cout<<"Create the assembly file error!"<<endl;
        return;
    }

    RC.FreeAll();

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

    switch(FirstChild(root)->type)
    {
        case PRINT_STATEMENT:
            CodeGenerate_Print_Statement(FirstChild(root));break;
        case VARIABLE_DEFINITION:
            CodeGenerate_Variable_Definition(FirstChild(root));break;
        case ASSIGNMENT_STATEMENT:
            CodeGenerate_Assignment_Statement(FirstChild(root));break;
        case COMPOUND_STATEMENT:
            CodeGenerate_Compound_Statement(FirstChild(root));break;
        case IF_STATEMENT:
            CodeGenerate_If_Statement(FirstChild(root));break;
        case ITERATION_STATEMENT:
            CodeGenerate_Iteration_Statement(FirstChild(root));break;
    }
}

void CodeGenerator::CodeGenerate_Compound_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Compound_Statement");

    for(int i=1;i<root->Children.size()-1;i++)
    {
        CodeGenerate_Statement(root->Children[i]);
    }
}



void CodeGenerator::CodeGenerate_Print_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Print_Statement");

    int expression_ri=CodeGenerate_Expression(root->Children[1]);
    Print(expression_ri);
}

void CodeGenerator::CodeGenerate_Assignment_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Assignment_Statement");

    string identifier=FirstChild(root)->lexeme;
    if(ST.Symbol_Exist(identifier))
    {
        int exp_i=CodeGenerate_Expression(root->Children[2]);
        Store(exp_i,identifier);
    }
    else CodeGenerate_Error("The identifier "+identifier+" is not defined.",FirstChild(root));
}

void CodeGenerator::CodeGenerate_Variable_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_Variable_Definition");

    string identifier=CodeGenerate_Variable_Declaration(FirstChild(root));
    
    if(root->Children[1]->type==AST_ASSIGN)
    {
        int exp_i=CodeGenerate_Expression(root->Children[2]);
        Store(exp_i,identifier);
    }
}
    
string CodeGenerator::CodeGenerate_Variable_Declaration(ASTNode* root)
{
    WhoAmI("CodeGenerate_Variable_Declaration");

    string identifier=root->Children[1]->lexeme;
    if(!ST.Symbol_Exist(identifier))
    {
        ST.Symbol_Add(identifier);
        CreateVar(identifier);
    }
    else CodeGenerate_Error("The identifier "+identifier+" is redefined.",root->Children[1]);
    
    return identifier; 
}



void CodeGenerator::CodeGenerate_If_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_If_Statement");

    int expression_ri=CodeGenerate_Expression(root->Children[2]);
   
    CompareZero(expression_ri);

    int lable1=NewLable();
    Jump("je",lable1);

    CodeGenerate_Statement(root->Children[4]);
    
    //have else 
    if(root->Children.size()>5&&root->Children[5]->type==AST_ELSE)
    {
        int lable2=NewLable();
        Jump("jmp",lable2);
        LablePrint(lable1);

        CodeGenerate_Statement(root->Children[6]);

        LablePrint(lable2);
    }
    else LablePrint(lable1); //only if
}



void CodeGenerator::CodeGenerate_Iteration_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Iteration_Statement");

    switch(FirstChild(root)->type)
    {
        case WHILE_STATEMENT:
            CodeGenerate_While_Statement(FirstChild(root));break;
        case DOWHILE_STATEMENT:
            CodeGenerate_DoWhile_Statement(FirstChild(root));break;
    }
}

void CodeGenerator::CodeGenerate_While_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_While_Statement");

    int lable1=NewLable();
    LablePrint(lable1);

    int expression_ri=CodeGenerate_Expression(root->Children[2]);
    CompareZero(expression_ri);

    int lable2=NewLable();
    Jump("je",lable2);

    CodeGenerate_Statement(root->Children[4]);
    Jump("jmp",lable1);

    LablePrint(lable2);
}

void CodeGenerator::CodeGenerate_DoWhile_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_DoWhile_Statement");

    int lable=NewLable();
    LablePrint(lable);

    CodeGenerate_Statement(root->Children[1]);

    int expression_ri=CodeGenerate_Expression(root->Children[4]);
    CompareZero(expression_ri);

    Jump("jne",lable);
}



int CodeGenerator::CodeGenerate_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression");

    return CodeGenerate_Equality_Expression(FirstChild(root));
}

int CodeGenerator::CodeGenerate_Equality_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Equality_Expression");

    int result_ri=CodeGenerate_Relational_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Relational_Expression(root->Children[i+1]);
        if(root->Children[i]->type==AST_EQUAL)result_ri=Equal(result_ri,temp_ri);
        else if(root->Children[i]->type==AST_NOT_EQUAL)result_ri=NotEqual(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_Relational_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Relational_Expression");

    int result_ri=CodeGenerate_PlusMinus_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_PlusMinus_Expression(root->Children[i+1]);
        switch(root->Children[i]->type)
        {
            case AST_LESS:result_ri=Less(result_ri,temp_ri);break;
            case AST_LESS_EQUAL:result_ri=LessEqual(result_ri,temp_ri);break;
            case AST_GREATER:result_ri=Greater(result_ri,temp_ri);break;
            case AST_GREATER_EQUAL:result_ri=GreaterEqual(result_ri,temp_ri);break;
        }
    }

    return result_ri;
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
    
    if(FirstChild(root)->type==AST_CONSTANT_INT)return Load(FirstChild(root)->literal);
    else if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;
        if(ST.Symbol_Exist(identifier))return Load(identifier);
        else CodeGenerate_Error("The identifier "+identifier+" is not defined.",FirstChild(root));
    }
    return -1;
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
    int register_i=RC.Alloc();

    OutFile<<"\tmov\t"<<RC.Name(register_i)<<", "<<value<<endl;
    return register_i;
}

int CodeGenerator::Load(string identifier)
{
    int register_i=RC.Alloc();

    OutFile<<"\tmov\t"<<RC.Name(register_i)<<", ["<<identifier<<"]"<<endl;
    return register_i;
}

void CodeGenerator::Store(int r_i,string identifier)
{
    OutFile<<"\tmov\t["<<identifier<<"], "<<RC.Name(r_i)<<endl;
    //Release the right value
    RC.Free(r_i);
}

void CodeGenerator::CreateVar(string identifier)
{
    OutFile<<"\tcommon\t"<<identifier<<"  8:8"<<endl;
}



int CodeGenerator::Add(int r1_i,int r2_i)
{
    OutFile<<"\tadd\t"<<RC.Name(r1_i)<<", "<<RC.Name(r2_i)<<endl;
    
    RC.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Sub(int r1_i,int r2_i)
{
    OutFile<<"\tsub\t"<<RC.Name(r1_i)<<", "<<RC.Name(r2_i)<<endl;
    
    RC.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Mul(int r1_i,int r2_i)
{
    OutFile<<"\timul\t"<<RC.Name(r1_i)<<", "<<RC.Name(r2_i)<<endl;

    RC.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Div(int r1_i,int r2_i)
{
    OutFile<<"\tmov\trax, "<<RC.Name(r1_i)<<endl;
    OutFile<<"\tcqo"<<endl;
    OutFile<<"\tidiv\t"<<RC.Name(r2_i)<<endl;
    OutFile<<"\tmov\t"<<RC.Name(r1_i)<<", rax"<<endl;

    RC.Free(r2_i);
    return r1_i;
}

void CodeGenerator::Print(int r_i)
{
    OutFile<<"\tpush\trbp"<<endl;

    OutFile<<"\tmov\trdi, format"<<endl;
    OutFile<<"\tmov\trsi, "<<RC.Name(r_i)<<endl;
    OutFile<<"\tmov\trax, 0"<<endl;
    OutFile<<"\tcall\tprintf"<<endl;

    OutFile<<"\tpop\trbp"<<endl;
    RC.Free(r_i);
}



int CodeGenerator::Compare(int r1_i,int r2_i,string setx)
{
    OutFile<<"\tcmp\t"<<RC.Name(r1_i)<<", "<<RC.Name(r2_i)<<endl;
    OutFile<<"\t"<<setx<<"\t"<<RC.Name(r1_i)<<'b'<<endl;
    OutFile<<"\tand\t"<<RC.Name(r1_i)<<", 255"<<endl;

    RC.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Equal(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"sete");
}

int CodeGenerator::NotEqual(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"setne");
}

int CodeGenerator::Less(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"setl");
} 

int CodeGenerator::LessEqual(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"setle");
}

int CodeGenerator::Greater(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"setg");
}

int CodeGenerator::GreaterEqual(int r1_i,int r2_i)
{
    return Compare(r1_i,r2_i,"setge");
}

void CodeGenerator::CompareZero(int r_i)
{
    OutFile<<"\tcmp\t"<<RC.Name(r_i)<<", 0"<<endl;
}


int CodeGenerator::NewLable()
{
    return ++LableNumber;
}
  
void CodeGenerator::LablePrint(int lable_number)
{
    OutFile<<"L"<<lable_number<<":"<<endl;
}

void CodeGenerator::Jump(string jump,int lable_numbr)
{
    OutFile<<"\t"<<jump<<"\t"<<"L"<<lable_numbr<<endl;
}



void CodeGenerator::CodeGenerate_Error(string error_message,ASTNode* root)
{
    cout<< "CodeGenerate Error: Line "<<root->line<<": "<<error_message<<endl;
    exit(4);
}

void CodeGenerator::CodeGenerate_Error(string error_message)
{
    cout<< "CodeGenerate Error: "<<": "<<error_message<<endl;
    exit(4);
}

void CodeGenerator::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}