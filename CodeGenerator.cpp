#include "CodeGenerator.h"

void CodeGenerator::CodeGenerate(string path)
{
    OutFile.open(path,ios::out);
    if(!OutFile.is_open())
    {
        cout<<"Create the assembly file error!"<<endl;
        return;
    }

    register_controller.FreeAll();

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
}



void CodeGenerator::CodeGenerate_Translation_Unit(ASTNode* root)
{
    WhoAmI("CodeGenerate_Translation_Unit");

    for(int i=0;i<root->Children.size();i++)
    {
        CodeGenerate_Function_Definition(root->Children[i]);
    }
}



void CodeGenerator::CodeGenerate_Function_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_Function_Definition");

    FunctionType type=CodeGenerate_Function_Type(FirstChild(root));
    string identifier=root->Children[1]->lexeme;

    if(!symbol_table.function_table.Function_Exist(identifier))
    {   
        int end_lable=NewLable();
        symbol_table.function_table.Function_Add(type,identifier,vector<Parameter>(),end_lable);
        
        NowInFunction=identifier;

        FunctionHead(root->Children[1]->lexeme);

        CodeGenerate_Compound_Statement(root->Children[4]);
        //Todo
        FunctionTail();
    }
    else CodeGenerate_Error("The function "+identifier+" is redefined.",root->Children[1]);

}

FunctionType CodeGenerator::CodeGenerate_Function_Type(ASTNode* root)
{
    WhoAmI("CodeGenerate_Function_Type");

    FunctionType ftype;

    switch(FirstChild(root)->type)
    {   
        case AST_VOID:ftype=F_VOID;break;
        case AST_INT:ftype=F_INT;break;
        case AST_CHAR:ftype=F_CHAR;break;
        case AST_LONG:ftype=F_LONG;break;
    }

    return ftype;
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
        case COMPOUND_STATEMENT:
            CodeGenerate_Compound_Statement(FirstChild(root));break;
        case IF_STATEMENT:
            CodeGenerate_If_Statement(FirstChild(root));break;
        case ITERATION_STATEMENT:
            CodeGenerate_Iteration_Statement(FirstChild(root));break;
        case EXPRESSION_STATEMENT:
            CodeGenerate_Expression_Statement(FirstChild(root));break;
        case RETURN_STATEMENT:
            CodeGenerate_Return_Statement(FirstChild(root));break;
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

    VariableType type=CodeGenerate_Variable_Type(FirstChild(root));
    string identifier=root->Children[1]->lexeme;

    if(!symbol_table.variable_table.Variable_Exist(identifier))
    {
        symbol_table.variable_table.Variable_Add(type,identifier);
        CreateVar(identifier);
    }
    else CodeGenerate_Error("The variable "+identifier+" is redefined.",root->Children[1]);
    
    return identifier; 
}

VariableType CodeGenerator::CodeGenerate_Variable_Type(ASTNode* root)
{
    WhoAmI("CodeGenerate_Variable_Type");

    VariableType vtype;

    switch(FirstChild(root)->type)
    {
        case AST_INT:vtype=V_INT;break;
        case AST_CHAR:vtype=V_CHAR;break;
        case AST_LONG:vtype=V_LONG;break;
    }

    return vtype;
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



void CodeGenerator::CodeGenerate_Expression_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression_Statement");

    if(FirstChild(root)->type==EXPRESSION)
    {
        int expression_ri=CodeGenerate_Expression(FirstChild(root));
        register_controller.Free(expression_ri);
    }
}



void CodeGenerator::CodeGenerate_Return_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Return_Statement");

    int expression_ri=-1;

//type check
    if(symbol_table.function_table.Function_Visit(NowInFunction).type==F_VOID)
    {
        if(root->Children[1]->type==EXPRESSION)
            CodeGenerate_Error("Return value does not match function type.",FirstChild(root));
    }
    else
    {
        if(root->Children[1]->type==AST_SEMICOLON)
            CodeGenerate_Error("Return value does not match function type.",FirstChild(root));
        else expression_ri=CodeGenerate_Expression(root->Children[1]);
    }

    Return(expression_ri,NowInFunction);

}



int CodeGenerator::CodeGenerate_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression");

    int result_ri=CodeGenerate_Assignment_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        register_controller.Free(result_ri);
        result_ri=CodeGenerate_Assignment_Expression(root->Children[i+1]);
    }
    return result_ri;
}

int CodeGenerator::CodeGenerate_Assignment_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Assignment_Expression");

    if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;
        if(symbol_table.variable_table.Variable_Exist(identifier))
        {
            int expression_ri=CodeGenerate_Equality_Expression(root->Children[2]);
            Store(expression_ri,identifier);
            return Load(identifier);
        }
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }
    
    else return  CodeGenerate_Equality_Expression(FirstChild(root));

    return -1;
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
    
    if(FirstChild(root)->type==FUNCTIONCALL_EXPRESSION)return CodeGenerate_FunctionCall_Expression(FirstChild(root));
    else if(FirstChild(root)->type==AST_CONSTANT_INT)return Load(FirstChild(root)->literal);
    else if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;
        if(symbol_table.variable_table.Variable_Exist(identifier))return Load(identifier);
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }

    return -1;
}

int CodeGenerator::CodeGenerate_FunctionCall_Expression(ASTNode* root)
{
    string identifier=FirstChild(root)->lexeme;
    int expression_ri=CodeGenerate_Expression(root->Children[2]);

    return FunctionCall(expression_ri,identifier);
}


void CodeGenerator::CodeGenerate_Tail()
{
    //OutFile<<endl<<"exit:"<<endl;
    //OutFile<<"\tmov\trax, 0"<<endl;
    //OutFile<<"\tret"<<endl<<endl;

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
    int register_i=register_controller.Alloc();

    OutFile<<"\tmov\t"<<register_controller.Name(register_i)<<", "<<value<<endl;
    return register_i;
}

int CodeGenerator::Load(string identifier)
{
    int register_i=register_controller.Alloc();

    switch(symbol_table.variable_table.Variable_Visit(identifier).type)
    {
        case V_INT:
            OutFile<<"\txor\t"<<register_controller.Name(register_i,8)<<", "<<register_controller.Name(register_i,8)<<endl;
            OutFile<<"\tmov\t"<<register_controller.Name(register_i,4)<<", dword ["<<identifier<<"]"<<endl;
            break;
        case V_CHAR:
            OutFile<<"\tmovzx\t"<<register_controller.Name(register_i,8)<<", byte ["<<identifier<<"]"<<endl;
            break;
        case V_LONG:
            OutFile<<"\tmov\t"<<register_controller.Name(register_i,8)<<", ["<<identifier<<"]"<<endl;
            break;
    }

    return register_i;
}

void CodeGenerator::Store(int r_i,string identifier)
{
    switch(symbol_table.variable_table.Variable_Visit(identifier).type)
    {
        case V_INT:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_controller.Name(r_i,4)<<endl;break;
        case V_CHAR:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_controller.Name(r_i,1)<<endl;break;
        case V_LONG:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_controller.Name(r_i,8)<<endl;break;
    }   
    //Release the right value
    register_controller.Free(r_i);
}

void CodeGenerator::CreateVar(string identifier)
{
    switch(symbol_table.variable_table.Variable_Visit(identifier).type)
    {
        case V_INT:
            OutFile<<"\tcommon\t"<<identifier<<"  4:4"<<endl;break;
        case V_CHAR:
            OutFile<<"\tcommon\t"<<identifier<<"  1:1"<<endl;break;
        case V_LONG:
            OutFile<<"\tcommon\t"<<identifier<<"  8:8"<<endl;break;
    }
}



int CodeGenerator::Add(int r1_i,int r2_i)
{
    OutFile<<"\tadd\t"<<register_controller.Name(r1_i)<<", "<<register_controller.Name(r2_i)<<endl;
    
    register_controller.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Sub(int r1_i,int r2_i)
{
    OutFile<<"\tsub\t"<<register_controller.Name(r1_i)<<", "<<register_controller.Name(r2_i)<<endl;
    
    register_controller.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Mul(int r1_i,int r2_i)
{
    OutFile<<"\timul\t"<<register_controller.Name(r1_i)<<", "<<register_controller.Name(r2_i)<<endl;

    register_controller.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Div(int r1_i,int r2_i)
{
    OutFile<<"\tmov\trax, "<<register_controller.Name(r1_i)<<endl;
    OutFile<<"\tcqo"<<endl;
    OutFile<<"\tidiv\t"<<register_controller.Name(r2_i)<<endl;
    OutFile<<"\tmov\t"<<register_controller.Name(r1_i)<<", rax"<<endl;

    register_controller.Free(r2_i);
    return r1_i;
}

void CodeGenerator::Print(int r_i)
{
    //OutFile<<"\tpush\trbp"<<endl;

    OutFile<<"\tmov\trdi, format"<<endl;
    OutFile<<"\tmov\trsi, "<<register_controller.Name(r_i)<<endl;
    OutFile<<"\tmov\trax, 0"<<endl;
    OutFile<<"\tcall\tprintf"<<endl;

    //OutFile<<"\tpop\trbp"<<endl;
    register_controller.Free(r_i);
}



int CodeGenerator::Compare(int r1_i,int r2_i,string setx)
{
    OutFile<<"\tcmp\t"<<register_controller.Name(r1_i)<<", "<<register_controller.Name(r2_i)<<endl;
    OutFile<<"\t"<<setx<<"\t"<<register_controller.Name(r1_i)<<'b'<<endl;
    OutFile<<"\tand\t"<<register_controller.Name(r1_i)<<", 255"<<endl;

    register_controller.Free(r2_i);
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
    OutFile<<"\tcmp\t"<<register_controller.Name(r_i)<<", 0"<<endl;
//Todo
    register_controller.Free(r_i);
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



void CodeGenerator::FunctionHead(string name)
{
    OutFile<<name<<":"<<endl;
    OutFile<<"\tpush\trbp"<<endl;
    OutFile<<"\tmov\trbp, rsp"<<endl;
}

void CodeGenerator::FunctionTail()
{
    LablePrint(symbol_table.function_table.Function_Visit(NowInFunction).end_lable);
    OutFile<<"\tpop\trbp"<<endl;
    OutFile<<"\tret"<<endl<<endl;
}

int CodeGenerator::FunctionCall(int r_i,string identifier)
{
    int out_ri=register_controller.Alloc();

    OutFile<<"\tmov\trdi, "<<register_controller.Name(r_i)<<endl;
    OutFile<<"\tcall\t"<<identifier<<endl;
    OutFile<<"\tmov\t"<<register_controller.Name(out_ri)<<", rax"<<endl;

    register_controller.Free(r_i);

    return out_ri;
}

void CodeGenerator::Return(int r_i,string identifier)
{
    switch(symbol_table.function_table.Function_Visit(identifier).type)
    {
        case F_VOID:break;
        case F_INT:
            OutFile<<"\tmov\teax, "<<register_controller.Name(r_i,4)<<endl;
            break;
        case F_CHAR:
            OutFile<<"\tmovzx\teax, "<<register_controller.Name(r_i,1)<<endl;
            break;
        case F_LONG:
            OutFile<<"\tmov\trax, "<<register_controller.Name(r_i,8)<<endl;
            break;
    }

    Jump("jmp",symbol_table.function_table.Function_Visit(identifier).end_lable);

    register_controller.Free(r_i);
}



void CodeGenerator::CodeGenerate_Error(string error_message,ASTNode* root)
{
    cout<< "CodeGenerate Error: Line "<<root->line<<": "<<error_message<<endl;
    exit(5);
}

void CodeGenerator::CodeGenerate_Error(string error_message)
{
    cout<< "CodeGenerate Error: "<<": "<<error_message<<endl;
    exit(5);
}

void CodeGenerator::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}