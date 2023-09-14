#include "CodeGenerator.h"

void CodeGenerator::CodeGenerate(string path)
{
    OutFile.open(path,ios::out);
    if(!OutFile.is_open())
    {
        cout<<"Create the assembly file error!"<<endl;
        return;
    }

    register_manager.FreeAll();

    cout<<endl<<"--- CodeGenerate Begin ---"<<endl;

    CodeGenerate_Head();
    CodeGenerate_Translation_Unit(ast.Translation);
    CodeGenerate_Tail();

    cout<<"--- CodeGenerate Done ---"<<endl<<endl;

    OutFile.close();
}

void CodeGenerator::CodeGenerate_Head()
{
    OutFile<<"\textern printint"<<endl<<endl;
}

void CodeGenerator::CodeGenerate_Tail()
{
    //OutFile<<"section .data"<<endl;
    //OutFile<<"\tformat: db \"%d\",0XA,0"<<endl;
}



// Begin
void CodeGenerator::CodeGenerate_Translation_Unit(ASTNode* root)
{
    WhoAmI("CodeGenerate_Translation_Unit");

    for(int i=0;i<root->Children.size();i++)
    {
        if(root->Children[i]->type==FUNCTION_DEFINITION)
            CodeGenerate_Function_Definition(root->Children[i]);
        else
            CodeGenerate_Variable_Definition(root->Children[i]);
    }
}



// Definition, Declaration
Type CodeGenerator::CodeGenerate_Type(ASTNode* root)
{
    WhoAmI("CodeGenerate_Type");

    Type type;

    switch(FirstChild(root)->type)
    {   
        case AST_VOID:type=T_VOID;break;
        case AST_CHAR:type=T_CHAR;break;
        case AST_INT:type=T_INT;break;
        case AST_LONG:type=T_LONG;break;
    }

    return type;
}

void CodeGenerator::CodeGenerate_Function_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_Function_Definition");

    Type type=CodeGenerate_Type(FirstChild(root));
    string identifier=root->Children[1]->lexeme;

    if(!function_table.Exist(identifier))
    {   
        int end_lable=NewLable();
        function_table.Add(type,identifier,vector<Parameter>(),end_lable);
        
        NowInFunction=identifier;

        FunctionHead(NowInFunction);

        CodeGenerate_Compound_Statement(root->Children[4]);
        
        FunctionTail(NowInFunction);
    }
    else CodeGenerate_Error("The function "+identifier+" is redefined.",root->Children[1]);
}



void CodeGenerator::CodeGenerate_Variable_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_Variable_Definition");

    string identifier=CodeGenerate_Variable_Declaration(FirstChild(root));
    
    if(root->Children[1]->type==AST_ASSIGN)
    {
        int expression_ri=CodeGenerate_Expression(root->Children[2]);
        Store(expression_ri,identifier);
    }
}
    
string CodeGenerator::CodeGenerate_Variable_Declaration(ASTNode* root)
{
    WhoAmI("CodeGenerate_Variable_Declaration");

    Type type=CodeGenerate_Type(FirstChild(root));
    //type check
    if(type==T_VOID&&root->Children[1]->type!=AST_STAR)CodeGenerate_Error("Illegal variable type",root->Children[1]);
    
    string identifier;
    //type cast
    if(root->Children[1]->type==AST_STAR)
    {
        type=Type_To_PtrType(type);
        identifier=root->Children[2]->lexeme;
    }
    else identifier=root->Children[1]->lexeme;

    if(!variable_table.Exist(identifier))
    {
        variable_table.Add(type,identifier);
        CreateVar(identifier);
    }
    else CodeGenerate_Error("The variable "+identifier+" is redefined.",root->Children[1]);
    
    return identifier; 
}



// Statement
void CodeGenerator::CodeGenerate_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Statement");

    switch(FirstChild(root)->type)
    {
        case PRINT_STATEMENT:
            CodeGenerate_Print_Statement(FirstChild(root));break;        
        case COMPOUND_STATEMENT:
            CodeGenerate_Compound_Statement(FirstChild(root));break;
        case VARIABLE_DEFINITION:
            CodeGenerate_Variable_Definition(FirstChild(root));break;
        case IF_STATEMENT:
            CodeGenerate_If_Statement(FirstChild(root));break;
        case ITERATION_STATEMENT:
            CodeGenerate_Iteration_Statement(FirstChild(root));break;  
        case RETURN_STATEMENT:
            CodeGenerate_Return_Statement(FirstChild(root));break;
        case EXPRESSION_STATEMENT:
            CodeGenerate_Expression_Statement(FirstChild(root));break;
    }
}



void CodeGenerator::CodeGenerate_Print_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Print_Statement");

    int expression_ri=CodeGenerate_Expression(root->Children[1]);
    Print(expression_ri);
}



void CodeGenerator::CodeGenerate_Compound_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Compound_Statement");

    for(int i=1;i<root->Children.size()-1;i++)
    {
        CodeGenerate_Statement(root->Children[i]);
    }
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



void CodeGenerator::CodeGenerate_Return_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Return_Statement");

    int expression_ri=-1;

//type check
    if(function_table.Visit(NowInFunction).type==T_VOID)
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



void CodeGenerator::CodeGenerate_Expression_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression_Statement");

    if(FirstChild(root)->type==EXPRESSION)
    {
        int expression_ri=CodeGenerate_Expression(FirstChild(root));
        
        //very very important
        register_manager.Free(expression_ri);
    }
}



// Expression
int CodeGenerator::CodeGenerate_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Expression");

    int result_ri=CodeGenerate_Assignment_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {   
        int temp_ri=CodeGenerate_Assignment_Expression(root->Children[i+1]);
        result_ri=Comma(result_ri,temp_ri);
    }
    return result_ri;
}

int CodeGenerator::CodeGenerate_Assignment_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Assignment_Expression");

    if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;
        if(variable_table.Exist(identifier))
        {
            int equality_expression_ri=CodeGenerate_Equality_Expression(root->Children[2]);
            Store(equality_expression_ri,identifier);
            return Load(identifier);
        }
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }
    
    return  CodeGenerate_Equality_Expression(FirstChild(root));
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


// int CodeGenerator::CodeGenerate_Unary_Expression(ASTNode* root)
// {
//     WhoAmI("CodeGenerate_Unary_Expression");
// //BUG
//     if(FirstChild(root)->type==AST_AMPERSAND)
//     {
//         if(FirstChild(root->Children[1])->type==AST_ID)
//         {
//             string identifier=FirstChild(root->Children[1])->lexeme;
//             if(variable_table.Exist(identifier))return Address(identifier);
//             else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root->Children[1]));
//         }
//     }
//     else if(FirstChild(root)->type==AST_STAR)
//     {
//         if(FirstChild(root->Children[1])->type==AST_ID)
//         {
//             string identifier=FirstChild(root->Children[1])->lexeme;

//             int primary_expression_ri=CodeGenerate_Primary_Expression(root->Children[1]);
// //BUG don't check pointer's type
//             if(variable_table.Exist(identifier))
//                 return Dereference(primary_expression_ri,variable_table.Visit(identifier).type);
//             else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root->Children[1]));
//         }
//     }
//     //else
//     return CodeGenerate_Primary_Expression(FirstChild(root));
// }
int CodeGenerator::CodeGenerate_Unary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Unary_Expression");

    return CodeGenerate_Primary_Expression(FirstChild(root));
}
    
int CodeGenerator::CodeGenerate_Primary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Primary_Expression");
    
    if(FirstChild(root)->type==FUNCTIONCALL_EXPRESSION)return CodeGenerate_FunctionCall_Expression(FirstChild(root));
    
    else if(FirstChild(root)->type==ADDRESS_EXPRESSION)return CodeGenerate_Address_Expression(FirstChild(root));
    else if(FirstChild(root)->type==DREFERENCE_EXPRESSION)return CodeGenerate_Dreference_Expression(FirstChild(root));

    else if(FirstChild(root)->type==AST_CONSTANT_INT)return Load(FirstChild(root)->literal);
    
    else if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;
        if(variable_table.Exist(identifier))return Load(identifier);
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }

    return -1;
}



int CodeGenerator::CodeGenerate_FunctionCall_Expression(ASTNode* root)
{
    string identifier=FirstChild(root)->lexeme;

//don't check for now
    //if(!function_table.Exist(identifier))
        //CodeGenerate_Error("The function "+identifier+" is not defined.",FirstChild(root));

    int expression_ri=CodeGenerate_Expression(root->Children[2]);

    return FunctionCall(expression_ri,identifier);
}



int CodeGenerator::CodeGenerate_Address_Expression(ASTNode* root)
{
    string identifier=root->Children[1]->lexeme;

    if(!variable_table.Exist(identifier))
        CodeGenerate_Error("The variable "+identifier+" is not defined.",root->Children[1]);
    
    //offset
    if(root->Children.size()>2)
    {
        int offset=root->Children[3]->literal;
    
        int r1_i=Address(identifier);
        int r2_i=Load(offset);

        int scale_factor=Address_ScaleFactor(variable_table.Visit(identifier).type);

        if(root->Children[2]->type==AST_PLUS)return Add(r1_i,ShiftLeft(r2_i,scale_factor));
        else if(root->Children[2]->type==AST_MINUS)return Sub(r1_i,ShiftLeft(r2_i,scale_factor));
    }

    //no offset
    return Address(identifier);
}

int CodeGenerator::CodeGenerate_Dreference_Expression(ASTNode* root)
{
    if(root->Children.size()>2)
    {
        string identifier=root->Children[2]->lexeme;

        if(!variable_table.Exist(identifier))
        CodeGenerate_Error("The variable "+identifier+" is not defined.",root->Children[1]);

        int offset=root->Children[4]->literal;

        int r1_i=Load(identifier);
        int r2_i=Load(offset);

        int scale_factor=Dreference_ScaleFactor(variable_table.Visit(identifier).type);

        Type type=variable_table.Visit(identifier).type;
        if(root->Children[3]->type==AST_PLUS)return Dereference(Add(r1_i,ShiftLeft(r2_i,scale_factor)),type);
        else if(root->Children[3]->type==AST_MINUS)return Dereference(Sub(r1_i,ShiftLeft(r2_i,scale_factor)),type);;
    }

    //no offset    
    string identifier=root->Children[1]->lexeme;

     if(!variable_table.Exist(identifier))
        CodeGenerate_Error("The variable "+identifier+" is not defined.",root->Children[1]);

    return Dereference(Load(identifier),variable_table.Visit(identifier).type);
}





//Atomic instruction
int CodeGenerator::Load(int value)
{
    int register_i=register_manager.Alloc();

    OutFile<<"\tmov\t"<<register_manager.Name(register_i)<<", "<<value<<endl;
    return register_i;
}

int CodeGenerator::Load(string identifier)
{
    int register_i=register_manager.Alloc();

    switch(variable_table.Visit(identifier).type)
    {
        case T_CHAR:
            OutFile<<"\tmovzx\t"<<register_manager.Name(register_i,8)<<", byte ["<<identifier<<"]"<<endl;
            break;
        case T_INT:
            OutFile<<"\txor\t"<<register_manager.Name(register_i,8)<<", "<<register_manager.Name(register_i,8)<<endl;
            OutFile<<"\tmov\t"<<register_manager.Name(register_i,4)<<", dword ["<<identifier<<"]"<<endl;
            break;
        case T_LONG:
        case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
            OutFile<<"\tmov\t"<<register_manager.Name(register_i,8)<<", ["<<identifier<<"]"<<endl;
            break;
        default:
            CodeGenerate_Error("Load variable "+identifier+" error.");
    }

    return register_i;
}



void CodeGenerator::Store(int r_i,string identifier)
{
    switch(variable_table.Visit(identifier).type)
    {
        case T_CHAR:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_manager.Name(r_i,1)<<endl;break;
        case T_INT:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_manager.Name(r_i,4)<<endl;break;
        case T_LONG:
        case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
            OutFile<<"\tmov\t["<<identifier<<"], "<<register_manager.Name(r_i,8)<<endl;break;
        default:
            CodeGenerate_Error("Store variable "+identifier+" error.");
    }   

    register_manager.Free(r_i);
}



void CodeGenerator::CreateVar(string identifier)
{
    OutFile<<"section\t.data"<<endl;
    OutFile<<"global\t"<<identifier<<endl;

    switch(variable_table.Visit(identifier).type)
    {        
        case T_CHAR:
            OutFile<<"\t"<<identifier<<":\tdb\t0"<<endl;break;
        case T_INT:
            OutFile<<"\t"<<identifier<<":\tdd\t0"<<endl;break;
        case T_LONG: 
        case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
             OutFile<<"\t"<<identifier<<":\tdq\t0"<<endl;break;
    }
    OutFile<<endl;
}



int CodeGenerator::Address(string identifier)
{
    int register_i=register_manager.Alloc();

    OutFile<<"\tmov\t"<<register_manager.Name(register_i)<<", "<<identifier<<endl;

    return register_i;
}

int CodeGenerator::Dereference(int r_i,Type ptr_type)
{
    switch(ptr_type)
    {
        case T_CHAR_PTR:
            OutFile<<"\tmovzx\t"<<register_manager.Name(r_i)<<", byte ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
        case T_INT_PTR:
        //BUG
            OutFile<<"\tmovzx\t"<<register_manager.Name(r_i)<<", word ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
        case T_LONG_PTR:
            OutFile<<"\tmov\t"<<register_manager.Name(r_i)<<", ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
    }

    return r_i;
}



//For consistency
int CodeGenerator::Comma(int r1_i,int r2_i)
{
    register_manager.Free(r1_i);
    return r2_i;
}



int CodeGenerator::Compare(int r1_i,int r2_i,string setx)
{
    OutFile<<"\tcmp\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;
    OutFile<<"\t"<<setx<<"\t"<<register_manager.Name(r1_i)<<'b'<<endl;
    OutFile<<"\tand\t"<<register_manager.Name(r1_i)<<", 255"<<endl;

    register_manager.Free(r2_i);
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
    OutFile<<"\tcmp\t"<<register_manager.Name(r_i)<<", 0"<<endl;

    register_manager.Free(r_i);
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



int CodeGenerator::Add(int r1_i,int r2_i)
{
    OutFile<<"\tadd\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;
    
    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Sub(int r1_i,int r2_i)
{
    OutFile<<"\tsub\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;
    
    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Mul(int r1_i,int r2_i)
{
    OutFile<<"\timul\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Div(int r1_i,int r2_i)
{
    OutFile<<"\tmov\trax, "<<register_manager.Name(r1_i)<<endl;
    OutFile<<"\tcqo"<<endl;
    OutFile<<"\tidiv\t"<<register_manager.Name(r2_i)<<endl;
    OutFile<<"\tmov\t"<<register_manager.Name(r1_i)<<", rax"<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}



int CodeGenerator::ShiftLeft(int r_i,int value)
{
    OutFile<<"\tsal\t"<<register_manager.Name(r_i)<<", "<<value<<endl;
    return r_i;
}



void CodeGenerator::FunctionHead(string identifier)
{
    OutFile<<"section\t.text"<<endl;
    OutFile<<"global\t"<<identifier<<endl;

    OutFile<<identifier<<":"<<endl;
    OutFile<<"\tpush\trbp"<<endl;
    OutFile<<"\tmov\trbp, rsp"<<endl;
}

void CodeGenerator::FunctionTail(string identifier)
{
    LablePrint(function_table.Visit(identifier).end_lable);
    OutFile<<"\tpop\trbp"<<endl;
    OutFile<<"\tret"<<endl<<endl;
}

int CodeGenerator::FunctionCall(int r_i,string identifier)
{
    int out_ri=register_manager.Alloc();

    OutFile<<"\tmov\trdi, "<<register_manager.Name(r_i)<<endl;
    OutFile<<"\tcall\t"<<identifier<<endl;
    OutFile<<"\tmov\t"<<register_manager.Name(out_ri)<<", rax"<<endl;

    register_manager.Free(r_i);

    return out_ri;
}

void CodeGenerator::Return(int r_i,string identifier)
{
    switch(function_table.Visit(identifier).type)
    {
        case T_VOID:break;        
        case T_CHAR:
            OutFile<<"\tmovzx\teax, "<<register_manager.Name(r_i,1)<<endl;
            break;
        case T_INT:
            OutFile<<"\tmov\teax, "<<register_manager.Name(r_i,4)<<endl;
            break;
        case T_LONG:
            OutFile<<"\tmov\trax, "<<register_manager.Name(r_i,8)<<endl;
            break;
    }

    Jump("jmp",function_table.Visit(identifier).end_lable);

    register_manager.Free(r_i);
}



void CodeGenerator::Print(int r_i)
{
    OutFile<<"\tmov\trdi, format"<<endl;
    OutFile<<"\tmov\trsi, "<<register_manager.Name(r_i)<<endl;
    OutFile<<"\tmov\trax, 0"<<endl;
    OutFile<<"\tcall\tprintf"<<endl;

    register_manager.Free(r_i);
}




//Tools
Type CodeGenerator::Type_To_PtrType(Type type)
{
    Type ptr_type;

    switch(type)
    {
        case T_CHAR:ptr_type=T_CHAR_PTR;break;
        case T_INT: ptr_type=T_INT_PTR;break;
        case T_LONG:ptr_type=T_LONG_PTR;break;
    }

    return ptr_type;
}



int CodeGenerator::Address_ScaleFactor(Type type)
{
    int factor=0;

    switch(type)
    {
        case T_CHAR:factor=0;break;
        case T_INT: factor=2;break;
        case T_LONG: factor=3;break;
    }

    return factor;
}

int CodeGenerator::Dreference_ScaleFactor(Type ptr_type)
{
    int factor=0;

    switch(ptr_type)
    {   
        case T_CHAR_PTR:factor=0;break;
        case T_INT_PTR:factor=2;break;
        case T_LONG_PTR:factor=3;break;
    }

    return factor;
}


ASTNode* CodeGenerator::FirstChild(ASTNode* root)
{
    if(root->Children.size()>=1)return root->Children[0];
    return nullptr;
}



void CodeGenerator::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}



void CodeGenerator::CodeGenerate_Error(string error_message)
{
    cout<< "CodeGenerate Error: "<<": "<<error_message<<endl;
    exit(5);
}

void CodeGenerator::CodeGenerate_Error(string error_message,ASTNode* root)
{
    cout<< "CodeGenerate Error: Line "<<root->line<<": "<<error_message<<endl;
    exit(5);
}
