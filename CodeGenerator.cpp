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
    OutFile<<"extern printf"<<endl;
    kissfunction_table.Add("printf");
    OutFile<<"extern scanf"<<endl;
    kissfunction_table.Add("scanf");
}

void CodeGenerator::CodeGenerate_Tail()
{
    if(TailData!="")
    {
        OutFile<<"section .data"<<endl;
        OutFile<<TailData<<endl;
    }
}



// Begin
void CodeGenerator::CodeGenerate_Translation_Unit(ASTNode* root)
{
    WhoAmI("CodeGenerate_Translation_Unit");

    for(int i=0;i<root->Children.size();i++)
    {
        if(root->Children[i]->type==KISS_DECLARATION)
        {
            CodeGenerate_Kiss_Declaration(root->Children[i]);
        }
    }
    OutFile<<endl;

    OutFile<<"section\t.data"<<endl;
    for(int i=0;i<root->Children.size();i++)
    {
        switch(root->Children[i]->type)
        {
            case GLOBALVARIABLE_DEFINITION:
                CodeGenerate_GlobalVariable_Definition(root->Children[i]);break;
            case GLOBALARRAY_DEFINITION:
                CodeGenerate_GlobalArray_Definition(root->Children[i]);break;
        }
    }
    OutFile<<endl;

    OutFile<<"section\t.text"<<endl;
    for(int i=0;i<root->Children.size();i++)
    {
        if(root->Children[i]->type==FUNCTION_DEFINITION)
        {
            CodeGenerate_Function_Definition(root->Children[i]);
        }
    }
    OutFile<<endl;
}



// Definition, Declaration
void CodeGenerator::CodeGenerate_Kiss_Declaration(ASTNode* root)
{
    WhoAmI("CodeGenerate_Kiss_Declaration");

    string identifier=root->Children[1]->lexeme;
    if(!kissfunction_table.Exist(identifier))
    {
        kissfunction_table.Add(identifier);
        OutFile<<"extern "<<identifier<<endl;
    }
}



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
    string identifier;
    int id_index=1;
    if(root->Children[1]->type==AST_SLASH)
    {
        type=Type_To_PtrType(type);
        id_index=2;
    }
    identifier=root->Children[id_index]->lexeme;

    if(kissfunction_table.Exist(identifier))
    {
        CodeGenerate_Error("The function "+identifier+" is declared as a kiss.",root->Children[1]);
    }

    if(!function_table.Exist(identifier))
    {   
        int end_lable=NewLable();
        NowInFunction=identifier;
        function_table.Add(type,identifier,end_lable);

        if(root->Children[id_index+2]->type==PARAMETER_LIST)
        {
            CodeGenerate_Parameter_List(root->Children[id_index+2]);

            Calculate_StackOffset(root->Children[id_index+4]);
            
            FunctionHead(NowInFunction);
            CodeGenerate_Compound_Statement(root->Children[id_index+4]);
        }   
        else
        {
            Calculate_StackOffset(root->Children[id_index+3]);

            FunctionHead(NowInFunction);
            CodeGenerate_Compound_Statement(root->Children[id_index+3]);
            
        }

        FunctionTail(NowInFunction);

    }
    else CodeGenerate_Error("The function "+identifier+" is redefined.",root->Children[1]);
}

void CodeGenerator::CodeGenerate_Parameter_List(ASTNode* root)
{
    WhoAmI("CodeGenerate_Parameter_List");

    for(int i=0;i<root->Children.size();i+=2)
    {
        CodeGenerate_LocalVariable_Declaration(root->Children[i],true);
    }
}



void CodeGenerator::CodeGenerate_GlobalVariable_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_GlobalVariable_Definition");

    Type type=CodeGenerate_Type(FirstChild(root));
    //type check
    if(type==T_VOID)CodeGenerate_Error("Illegal variable type",root->Children[1]);
    
    string identifier;
    int id_index=1;
    //type cast
    if(root->Children[1]->type==AST_STAR)
    {
        type=Type_To_PtrType(type);
        id_index=2;
    }
    identifier=root->Children[id_index]->lexeme;

    Guarantee_InExist_GlobalVartable(identifier,root->Children[id_index]);
    global_vartable.Add(type,identifier);
    
    int initialization_value=0;

    if(root->Children[id_index+1]->type==AST_ASSIGN)
    {
        ASTNode* constant_node=root->Children[id_index+2];
        if(constant_node->type==AST_CONSTANT_INT)
        {
            initialization_value=constant_node->literal_int;
        }
        else if(constant_node->type==AST_CONSTANT_CHAR)
        {
            initialization_value=constant_node->literal_char;
        }
    }

    CreateGlobalVar(identifier,initialization_value);
}

void CodeGenerator::CodeGenerate_GlobalArray_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_GlobalArray_Definition");

    Type type=CodeGenerate_Type(FirstChild(root));
    //type cast
    if(type==T_VOID)CodeGenerate_Error("Illegal array type",root->Children[1]);

    type=Type_To_ArrayType(type);

    string identifier=root->Children[1]->lexeme;

    Guarantee_InExist_GlobalVartable(identifier,root->Children[1]);
    
    int elem_n=root->Children[3]->literal_int;

    global_vartable.Add(type,identifier,elem_n);

    vector<int>initializer_list;

    if(root->Children[5]->type==AST_ASSIGN)
    {
        initializer_list=CodeGenerate_Initialize_List(root->Children[6]);
    }

    CreateGlobalArray(identifier,initializer_list);
}

vector<int> CodeGenerator::CodeGenerate_Initialize_List(ASTNode* root)
{
    WhoAmI("CodeGenerate_Initialize_List");

    vector<int>initialize_list;

    for(int i=1;i<root->Children.size();i+=2)
    {
        if(root->Children[i]->type==AST_CONSTANT_INT)
        {
            initialize_list.push_back(root->Children[i]->literal_int);
        }
        else if(root->Children[i]->type==AST_CONSTANT_CHAR)
        {
            initialize_list.push_back(root->Children[i]->literal_char);
        }
    }

    return initialize_list;
}




void CodeGenerator::CodeGenerate_LocalVariable_Definition(ASTNode* root)
{
    WhoAmI("CodeGenerate_LocalVariable_Definition");

    string identifier=CodeGenerate_LocalVariable_Declaration(FirstChild(root),false);
    
    if(root->Children[1]->type==AST_ASSIGN)
    {
        int expression_ri=CodeGenerate_Expression(root->Children[2]);
        StoreLocalVar(expression_ri,identifier,true);
    }

}

string CodeGenerator::CodeGenerate_LocalVariable_Declaration(ASTNode* root,bool is_parameter)
{
    WhoAmI("CodeGenerate_LocalVariable_Declaration");

    Type type=CodeGenerate_Type(FirstChild(root));
    //type check
    if(type==T_VOID)CodeGenerate_Error("Illegal variable type",root->Children[1]);
    
    string identifier;
    //type cast
    if(root->Children[1]->type==AST_STAR)
    {
        type=Type_To_PtrType(type);
        identifier=root->Children[2]->lexeme;
    }
    else identifier=root->Children[1]->lexeme;

    Guarantee_InExist_LocalVartable(identifier,root->Children[1]);
    
    int stack_offset=CreateLocalVar(type,identifier,is_parameter);
    Local_Vartable().Add(type,identifier,1,stack_offset);
    
    return identifier;
}




// Statement
void CodeGenerator::CodeGenerate_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Statement");

    switch(FirstChild(root)->type)
    {
        case COMPOUND_STATEMENT:
            CodeGenerate_Compound_Statement(FirstChild(root));break;
        case LOCALVARIABLE_DEFINITION:
            CodeGenerate_LocalVariable_Definition(FirstChild(root));break;
        case IF_STATEMENT:
            CodeGenerate_If_Statement(FirstChild(root));break;
        case ITERATION_STATEMENT:
            CodeGenerate_Iteration_Statement(FirstChild(root));break;  
        case CONTINUE_STATEMENT:
            CodeGenerate_Continue_Statement(FirstChild(root));break;
        case BREAK_STATEMENT:
            CodeGenerate_Break_Statement(FirstChild(root));break;
        case RETURN_STATEMENT:
            CodeGenerate_Return_Statement(FirstChild(root));break;
        case EXPRESSION_STATEMENT:
            CodeGenerate_Expression_Statement(FirstChild(root));break;
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
        case FOR_STATEMENT:
            CodeGenerate_For_Statement(FirstChild(root));break;
    }
}

void CodeGenerator::CodeGenerate_While_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_While_Statement");

    int begin_lable=NewLable();
    int end_lable=NewLable();

    iteration_manger.Enter(I_WHILE,begin_lable,end_lable);

    LablePrint(begin_lable);

    int expression_ri=CodeGenerate_Expression(root->Children[2]);
    CompareZero(expression_ri);
    
    Jump("je",end_lable);

    CodeGenerate_Statement(root->Children[4]);
    Jump("jmp",begin_lable);

    LablePrint(end_lable);

    iteration_manger.Leave();
}

void CodeGenerator::CodeGenerate_DoWhile_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_DoWhile_Statement");

    int begin_lable=NewLable();
    int intermediate_lable=NewLable();
    int end_lable=NewLable();

    iteration_manger.Enter(I_DOWHILE,begin_lable,intermediate_lable,end_lable);

    LablePrint(begin_lable);

    CodeGenerate_Statement(root->Children[1]);

    LablePrint(intermediate_lable);

    int expression_ri=CodeGenerate_Expression(root->Children[4]);
    CompareZero(expression_ri);

    Jump("jne",begin_lable);

    LablePrint(end_lable);

    iteration_manger.Leave();
}

void CodeGenerator::CodeGenerate_For_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_For_Statement");

    CodeGenerate_Expression_Statement(root->Children[2]);

    int begin_lable=NewLable();
    int intermediate_lable=NewLable();
    int end_lable=NewLable();

    iteration_manger.Enter(I_FOR,begin_lable,intermediate_lable,end_lable);

    LablePrint(begin_lable);

    if(root->Children[3]->Children[0]->type==EXPRESSION)
    {
        int expression_ri=CodeGenerate_Expression(root->Children[3]->Children[0]);
        CompareZero(expression_ri);
        Jump("je",end_lable);
    }

    if(root->Children[4]->type==EXPRESSION)
    {
        CodeGenerate_Statement(root->Children[6]);
        
        LablePrint(intermediate_lable);

        int expression_ri=CodeGenerate_Expression(root->Children[4]);
        register_manager.Free(expression_ri);
    }
    else 
    {
        CodeGenerate_Statement(root->Children[5]);

        LablePrint(intermediate_lable);
    }

    Jump("jmp",begin_lable);

    LablePrint(end_lable);

    iteration_manger.Leave();
}



void CodeGenerator::CodeGenerate_Continue_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Continue_Statement");

    switch(iteration_manger.NowInIteration().type)
    {
        case I_WHILE:    
            Jump("jmp",iteration_manger.NowInIteration().begin_lable);break;
        case I_DOWHILE:case I_FOR:
            Jump("jmp",iteration_manger.NowInIteration().intermediate_lable);break;
    }
}

void CodeGenerator::CodeGenerate_Break_Statement(ASTNode* root)
{
    WhoAmI("CodeGenerate_Break_Statement");

    Jump("jmp",iteration_manger.NowInIteration().end_lable);
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
//identifier
    if(root->Children.size()==3&&root->Children[1]->type==AST_ASSIGN)
    {
        string identifier=FirstChild(root)->lexeme;

        if(Local_Vartable().Exist(identifier))
        {
            int Conditional_expression_ri=CodeGenerate_Conditional_Expression(root->Children[2]);
            StoreLocalVar(Conditional_expression_ri,identifier,false);
            return Conditional_expression_ri;
        }
        else if(global_vartable.Exist(identifier))
        {
            int Conditional_expression_ri=CodeGenerate_Conditional_Expression(root->Children[2]);
            StoreGlobalVar(Conditional_expression_ri,identifier,false);
            return Conditional_expression_ri;
        }
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }
//*identifier    
    else if(root->Children.size()==4&&root->Children[2]->type==AST_ASSIGN)
    {
        string identifier=root->Children[1]->lexeme;
        if(Local_Vartable().Exist(identifier))
        {
            int Conditional_expression_ri=CodeGenerate_Conditional_Expression(root->Children[3]);
            Type type=Local_Vartable().Visit(identifier).type;
            Store(Conditional_expression_ri,LoadLocalVar(identifier),type,false);
            return Conditional_expression_ri;
        }
        else if(global_vartable.Exist(identifier))
        {
            int Conditional_expression_ri=CodeGenerate_Conditional_Expression(root->Children[3]);
            Type type=global_vartable.Visit(identifier).type;
            Store(Conditional_expression_ri,LoadGlobalVar(identifier),type,false);
            return Conditional_expression_ri;
        }
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }
//identifier[]
    else if(root->Children.size()==6&&root->Children[4]->type==AST_ASSIGN)
    {
        string identifier=FirstChild(root)->lexeme;
        Guarantee_Exist_GlobalVartable(identifier,FirstChild(root));

        int Conditional_expression_ri=CodeGenerate_Conditional_Expression(root->Children[5]);
        int offset_ri=CodeGenerate_Expression(root->Children[2]);
        Type type=global_vartable.Visit(identifier).type;
        int scale_factor=Dreference_ScaleFactor(type);
        
        Store(Conditional_expression_ri,Add(AddressGlobalVar(identifier),ShiftLeftConstant(offset_ri,scale_factor)),type,false);

        return Conditional_expression_ri;
    }

    return  CodeGenerate_Conditional_Expression(FirstChild(root));
}



int CodeGenerator::CodeGenerate_Conditional_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Conditional_Expression");

    if(root->Children.size()==5&&root->Children[1]->type==AST_QUESTION&&root->Children[3]->type==AST_COLON)
    {
        int result_ri=Load(0);

        int condition_ri=CodeGenerate_LogicOr_Expression(FirstChild(root));
        CompareZero(condition_ri);

        int lable1=NewLable();
        int lable2=NewLable();

        Jump("je",lable1);

        int value1_ri=CodeGenerate_Expression(root->Children[2]);
        Copy(result_ri,value1_ri);
        Jump("jmp",lable2);

        LablePrint(lable1);
        int value2_ri=CodeGenerate_Expression(root->Children[4]);
        Copy(result_ri,value2_ri);

        LablePrint(lable2);

        return result_ri;
    }

    return  CodeGenerate_LogicOr_Expression(FirstChild(root));
}



int CodeGenerator::CodeGenerate_LogicOr_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_LogicOr_Expression");

    int result_ri=CodeGenerate_LogicAnd_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_LogicAnd_Expression(root->Children[i+1]);
        result_ri=Or(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_LogicAnd_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_LogicAnd_Expression");

    int result_ri=CodeGenerate_Or_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Or_Expression(root->Children[i+1]);
        result_ri=And(result_ri,temp_ri);
    }

    return result_ri;
}



int CodeGenerator::CodeGenerate_Or_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Or_Expression");

    int result_ri=CodeGenerate_Xor_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Xor_Expression(root->Children[i+1]);
        result_ri=Or(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_Xor_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Xor_Expression");

    int result_ri=CodeGenerate_And_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_And_Expression(root->Children[i+1]);
        result_ri=Xor(result_ri,temp_ri);
    }

    return result_ri;
}

int CodeGenerator::CodeGenerate_And_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_And_Expression");

    int result_ri=CodeGenerate_Equality_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Equality_Expression(root->Children[i+1]);
        result_ri=And(result_ri,temp_ri);
    }

    return result_ri;
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

    int result_ri=CodeGenerate_Shift_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_Shift_Expression(root->Children[i+1]);
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



int CodeGenerator::CodeGenerate_Shift_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Shift_Expression");

    int result_ri=CodeGenerate_PlusMinus_Expression(FirstChild(root));

    for(int i=1;i<root->Children.size();i+=2)
    {
        int temp_ri=CodeGenerate_PlusMinus_Expression(root->Children[i+1]);

        if(root->Children[i]->type==AST_LEFT_SHIFT)result_ri=ShiftLeft(result_ri,temp_ri);
        else if(root->Children[i]->type==AST_RIGHT_SHIFT)result_ri=ShiftRight(result_ri,temp_ri);
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
        else if(root->Children[i]->type==AST_MOD)result_ri=Mod(result_ri,temp_ri);
    }

    return result_ri;
}



int CodeGenerator::CodeGenerate_Unary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Unary_Expression");

//no prefix
    if(FirstChild(root)->type==PRIMARY_EXPRESSION)
    {
        return CodeGenerate_Primary_Expression(FirstChild(root));
    }

//have prefix
    int result_ri=CodeGenerate_Primary_Expression(root->Children[1]);

    switch(FirstChild(root)->type)
    {
        case AST_MINUS:
            result_ri=Negate(result_ri);break;
        case AST_INVERT:
            result_ri=Invert(result_ri);break;
        case AST_NOT:
            result_ri=Not(result_ri);break;
    }
    return result_ri;
}


int CodeGenerator::CodeGenerate_Primary_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Primary_Expression");
    

    if(FirstChild(root)->type==FUNCTIONCALL_EXPRESSION)return CodeGenerate_FunctionCall_Expression(FirstChild(root));

    else if(FirstChild(root)->type==ADDRESS_EXPRESSION)return CodeGenerate_Address_Expression(FirstChild(root));
    else if(FirstChild(root)->type==DREFERENCE_EXPRESSION)return CodeGenerate_Dreference_Expression(FirstChild(root));
    else if(FirstChild(root)->type==ARRAY_EXPRESSION)return CodeGenerate_Array_Expression(FirstChild(root));

    else if(FirstChild(root)->type==SIZEOF_EXPRESSION)return CodeGenerate_Sizeof_Expression(FirstChild(root));

    else if(FirstChild(root)->type==INCDECPREFIX_EXPRESSION)return CodeGenerate_IncDecPrefix_Expression(FirstChild(root));
    else if(FirstChild(root)->type==INCDECPOSTFIX_EXPRESSION)return CodeGenerate_IncDecPostfix_Expression(FirstChild(root));

    else if(FirstChild(root)->type==AST_LEFT_PAREN)return CodeGenerate_Expression(root->Children[1]);

    else if(FirstChild(root)->type==AST_CONSTANT_INT)return Load(FirstChild(root)->literal_int);
    else if(FirstChild(root)->type==AST_CONSTANT_CHAR)return Load(FirstChild(root)->literal_char);
    else if(FirstChild(root)->type==AST_CONSTANT_STRING)return CreateString(FirstChild(root)->literal_string);
    
    else if(FirstChild(root)->type==AST_ID)
    {
        string identifier=FirstChild(root)->lexeme;

        if(Local_Vartable().Exist(identifier))return LoadLocalVar(identifier);
        else if(global_vartable.Exist(identifier))return LoadGlobalVar(identifier);
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));
    }

    return -1;
}



int CodeGenerator::CodeGenerate_FunctionCall_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_FunctionCall_Expression");
    
    string identifier=FirstChild(root)->lexeme;

    if(!function_table.Exist(identifier)&&!kissfunction_table.Exist(identifier))
    {
        CodeGenerate_Error("The function "+identifier+" is not defined.",FirstChild(root));
    }

    if(root->Children[2]->type==EXPRESSION)
    {
        ASTNode* expression_node=root->Children[2];

        if(function_table.Exist(identifier))
        {
            int arguments_size=(expression_node->Children.size()+1)/2;
            if(arguments_size!=function_table.Visit(identifier).parameter_list.size())
            {
               CodeGenerate_Error("Function call arguments'amount is not match with the function definition.");
            }
        }

        StoreArgument(expression_node);
    }

    return FunctionCall(identifier);
}



int CodeGenerator::CodeGenerate_Address_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Address_Expression");

    string identifier=root->Children[1]->lexeme;

//Local
    if(Local_Vartable().Exist(identifier))
    {
        return AddressLocalVar(identifier);
    }

//Global
    Guarantee_Exist_GlobalVartable(identifier,root->Children[1]);
    
    //array
    if(root->Children.size()>2&&root->Children[2]->type==AST_LEFT_SQUARE)
    {    
        int r1_i=AddressGlobalVar(identifier);
        int r2_i=CodeGenerate_Expression(root->Children[3]);

        int scale_factor=Address_ScaleFactor(global_vartable.Visit(identifier).type);

        return Add(r1_i,ShiftLeftConstant(r2_i,scale_factor));
    }

    //normal variable
    return AddressGlobalVar(identifier);
}

int CodeGenerator::CodeGenerate_Dreference_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Dreference_Expression");

    string identifier=root->Children[1]->lexeme;
//Local
    if(Local_Vartable().Exist(identifier))
    {
        return Dereference(LoadLocalVar(identifier),Local_Vartable().Visit(identifier).type);
    }
//Global
    Guarantee_Exist_GlobalVartable(identifier,root->Children[1]);

    return Dereference(LoadGlobalVar(identifier),global_vartable.Visit(identifier).type);
}

int CodeGenerator::CodeGenerate_Array_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Array_Expression");

    string identifier=FirstChild(root)->lexeme;

    Guarantee_Exist_GlobalVartable(identifier,FirstChild(root));

    int r1_i=AddressGlobalVar(identifier);
    int r2_i=CodeGenerate_Expression(root->Children[2]);

    Type type=global_vartable.Visit(identifier).type;
    int scale_factor=Dreference_ScaleFactor(type);

    return Dereference(Add(r1_i,ShiftLeftConstant(r2_i,scale_factor)),type);
}



int CodeGenerator::CodeGenerate_Sizeof_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_Sizeof_Expression");

    int result_size=0;

    if(root->Children[2]->type==AST_ID)
    {
        string identifier=root->Children[2]->lexeme;
        int elem_n=0;
        int elem_size=0;
        Type type;

        if(global_vartable.Exist(identifier))
        {
            type=global_vartable.Visit(identifier).type;
            elem_n=global_vartable.Visit(identifier).elem_n;
        }
        else if(Local_Vartable().Exist(identifier))
        {
            type=Local_Vartable().Visit(identifier).type;
            elem_n=Local_Vartable().Visit(identifier).elem_n;
        }
        else CodeGenerate_Error("The variable "+identifier+" is not defined.",root->Children[2]);

        switch(type)
        {
            case T_CHAR:        case T_CHAR_ARRAY:
                elem_size=1;break;
            case T_INT:         case T_INT_ARRAY:
                elem_size=4;break;
            case T_LONG:        case T_LONG_ARRAY:
            case T_CHAR_PTR:    case T_INT_PTR:     case T_LONG_PTR:
                elem_size=8;break;
        }

        result_size=elem_n*elem_size;
    }
    else
    {
        Type type=CodeGenerate_Type(root->Children[2]);
        //type check
        if(type==T_VOID)CodeGenerate_Error("Illegal variable type",root->Children[2]);

        switch(type)
        {
            case T_CHAR:result_size=1;break;
            case T_INT:result_size=4;break;
            case T_LONG:result_size=8;break;
        }

        if(root->Children[3]->type==AST_STAR)
        {
            result_size=8;
        }
    }

    return Load(result_size);
}



int CodeGenerator::CodeGenerate_IncDecPrefix_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_IncDecPrefix_Expression");

    ASTNodeType op=FirstChild(root)->type;
    string identifier=root->Children[1]->lexeme;
    
    int result_ri=-1;
    
    if(Local_Vartable().Exist(identifier))
    {
        if(op==AST_INC)result_ri=IncLocalVar(identifier,"pre");
        else if(op==AST_DEC)result_ri=DecLocalVar(identifier,"pre");
    }
    else if(global_vartable.Exist(identifier))
    {
        if(op==AST_INC)result_ri=IncGlobalVar(identifier,"pre");
        else if(op==AST_DEC)result_ri=DecGlobalVar(identifier,"pre");
    }
    else CodeGenerate_Error("The variable "+identifier+" is not defined.",FirstChild(root));

    return result_ri;
}

int CodeGenerator::CodeGenerate_IncDecPostfix_Expression(ASTNode* root)
{
    WhoAmI("CodeGenerate_IncDecPostfix_Expression");

    ASTNodeType op=root->Children[1]->type;
    string identifier=FirstChild(root)->lexeme;
    
    int result_ri=-1;
    
    if(Local_Vartable().Exist(identifier))
    {
        if(op==AST_INC)result_ri=IncLocalVar(identifier,"post");
        else if(op==AST_DEC)result_ri=DecLocalVar(identifier,"post");
    }
    else if(global_vartable.Exist(identifier))
    {
        if(op==AST_INC)result_ri=IncGlobalVar(identifier,"post");
        else if(op==AST_DEC)result_ri=DecGlobalVar(identifier,"post");
    }
    else CodeGenerate_Error("The variable "+identifier+" is not defined.hhh",FirstChild(root));

    return result_ri;
}


//Atomic instruction
int CodeGenerator::Load(int constant_value)
{
    int register_i=register_manager.Alloc();

    OutFile<<"\tmov\t"<<register_manager.Name(register_i)<<", "<<constant_value<<endl;
    return register_i;
}

int CodeGenerator::LoadGlobalVar(string identifier)
{
    int register_i=register_manager.Alloc();

    switch(global_vartable.Visit(identifier).type)
    {
        case T_CHAR:
            OutFile<<"\tmovzx\t"<<register_manager.Name(register_i,8)<<", byte ["<<identifier<<"]"<<endl;
            break;
        case T_INT:
            OutFile<<"\tmovsxd\t"<<register_manager.Name(register_i,8)<<", dword ["<<identifier<<"]"<<endl;
            break;
        case T_LONG:
        case T_CHAR_PTR:    case T_INT_PTR:     case T_LONG_PTR:
            OutFile<<"\tmov\t"<<register_manager.Name(register_i,8)<<", ["<<identifier<<"]"<<endl;
            break;
        case T_CHAR_ARRAY:  case T_INT_ARRAY:   case T_LONG_ARRAY:
            OutFile<<"\tmov\t"<<register_manager.Name(register_i,8)<<", "<<identifier<<endl;
            break;
            
        default:
            CodeGenerate_Error("Load variable "+identifier+" error.");
    }

    return register_i;
}

int CodeGenerator::LoadLocalVar(string identifier)
{
    int register_i=register_manager.Alloc();

    Type local_variable_type=Local_Vartable().Visit(identifier).type;
    int local_variable_stack_offset=Local_Vartable().Visit(identifier).stack_offset;

    switch(local_variable_type)
    {
        case T_CHAR:
            OutFile<<"\tmovzx\t"<<register_manager.Name(register_i,8)<<", byte [rbp+"<<local_variable_stack_offset<<"]"<<endl;
            break;
        case T_INT:
            //OutFile<<"\txor\t"<<register_manager.Name(register_i,8)<<", "<<register_manager.Name(register_i,8)<<endl;
            //OutFile<<"\tmov\t"<<register_manager.Name(register_i,4)<<", dword [rbp+"<<local_variable_stack_offset<<"]"<<endl;
            OutFile<<"\tmovsxd\t"<<register_manager.Name(register_i,8)<<", dword [rbp+"<<local_variable_stack_offset<<"]"<<endl;
            break;
        case T_LONG:
        case T_CHAR_PTR:    case T_INT_PTR:     case T_LONG_PTR:
            OutFile<<"\tmov\t"<<register_manager.Name(register_i,8)<<", [rbp+"<<local_variable_stack_offset<<"]"<<endl;
            break;
                     
        default:
            CodeGenerate_Error("Load variable "+identifier+" error.");
    }

    return register_i;
}



void CodeGenerator::StoreGlobalVar(int r_i,string identifier,bool free)
{
    switch(global_vartable.Visit(identifier).type)
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


    if(free)register_manager.Free(r_i);
}

void CodeGenerator::StoreLocalVar(int r_i,string identifier,bool free)
{
    Type local_variable_type=Local_Vartable().Visit(identifier).type;
    int local_variable_stack_offset=Local_Vartable().Visit(identifier).stack_offset;
    
    switch(local_variable_type)
    {
        case T_CHAR:
            OutFile<<"\tmov\tbyte [rbp+"<<local_variable_stack_offset<<"], "<<register_manager.Name(r_i,1)<<endl;break;
        case T_INT:
            OutFile<<"\tmov\tdword [rbp+"<<local_variable_stack_offset<<"], "<<register_manager.Name(r_i,4)<<endl;break;
        case T_LONG:
        case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
            OutFile<<"\tmov\tqword [rbp+"<<local_variable_stack_offset<<"], "<<register_manager.Name(r_i,8)<<endl;break;
        default:
            CodeGenerate_Error("Store variable "+identifier+" error.");
    }

    if(free)register_manager.Free(r_i);
}

void CodeGenerator::Store(int r1_i,int r2_i,Type type,bool free)
{
    switch(type)
    {
        case T_CHAR_PTR:case T_CHAR_ARRAY:
            OutFile<<"\tmov\t["<<register_manager.Name(r2_i)<<"], "<<register_manager.Name(r1_i,1)<<endl;break;
        case T_INT_PTR:case T_INT_ARRAY:
            OutFile<<"\tmov\t["<<register_manager.Name(r2_i)<<"], "<<register_manager.Name(r1_i,4)<<endl;break;
        case T_LONG_PTR:case T_LONG_ARRAY:
            OutFile<<"\tmov\t["<<register_manager.Name(r2_i)<<"], "<<register_manager.Name(r1_i,8)<<endl;break;
        default:
            CodeGenerate_Error("Store variable  error.");
    }   

    register_manager.Free(r2_i);
    if(free)register_manager.Free(r1_i);
}



void CodeGenerator::CreateGlobalVar(string identifier,int initialization_value)
{
    OutFile<<"global\t"<<identifier<<endl;
        
    switch(global_vartable.Visit(identifier).type)
    {        
        case T_CHAR:
            OutFile<<"\t"<<identifier<<":\tdb\t"<<initialization_value<<endl;break;
        case T_INT:
            OutFile<<"\t"<<identifier<<":\tdd\t"<<initialization_value<<endl;break;
        case T_LONG: 
        case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
            OutFile<<"\t"<<identifier<<":\tdq\t"<<initialization_value<<endl;break;
    }

    OutFile<<endl;
}

void CodeGenerator::CreateGlobalArray(string identifier,vector<int> initialize_list)
{    
    OutFile<<"global\t"<<identifier<<endl;

    int elem_n=global_vartable.Visit(identifier).elem_n;
    Type type=global_vartable.Visit(identifier).type;

    if(initialize_list.size()==0)
    {
        switch(type)
        {        
            case T_CHAR_ARRAY:
                OutFile<<"\t"<<identifier<<":\ttimes "<<elem_n<<" db\t0"<<endl;break;
            case T_INT_ARRAY:
                OutFile<<"\t"<<identifier<<":\ttimes "<<elem_n<<" dd\t0"<<endl;break;
            case T_LONG_ARRAY:
                OutFile<<"\t"<<identifier<<":\ttimes "<<elem_n<<" dq\t0"<<endl;break;
        }
    }
    else
    {
        switch(type)
        {        
            case T_CHAR_ARRAY:
                OutFile<<"\t"<<identifier<<":\tdb\t";break;
            case T_INT_ARRAY:
                OutFile<<"\t"<<identifier<<":\tdd\t";break;
            case T_LONG_ARRAY:
                OutFile<<"\t"<<identifier<<":\tdq\t";break;
        }

        for(int i=0;i<initialize_list.size();i++)
        {
            if(i>0)OutFile<<",";
            OutFile<<initialize_list[i];
        }

        for(int i=0;i<elem_n-initialize_list.size();i++)
        {
            OutFile<<",0";
        }
        
        OutFile<<endl;
    }

    OutFile<<endl;
}

int CodeGenerator::CreateLocalVar(Type type,string identifier,bool is_parameter)
{   
    int new_var_stack_offset=0;
    switch(type)
    {
        case T_CHAR:case T_INT:
            new_var_stack_offset=4;break;
        case T_LONG:
        case T_CHAR_PTR:case T_INT_PTR:case T_LONG_PTR:
            new_var_stack_offset=8;break;
    }

    Now_Function().local_offset+=new_var_stack_offset; 

    if(is_parameter)
    {
        Now_Function().stack_align_offset+=new_var_stack_offset;
        Now_Function().parameter_list.push_back(Parameter(type,identifier));
    }

    return -Now_Function().local_offset;
}



int CodeGenerator::CreateString(string literal_string)
{
    string string_name="Str"+to_string(NewString());

    TailData+="\t"+string_name+" db "+StringToIntlist(literal_string)+",0\n";
    
    int register_i=register_manager.Alloc();
    
    OutFile<<"\tmov\t"<<register_manager.Name(register_i)<<", "<<string_name<<endl;

    return register_i;
}

string CodeGenerator::StringToIntlist(string liter_string)
{
    string int_list;
    for(int i=0;i<liter_string.size();i++)
    {
        if(i>0)int_list.push_back(',');
        int_list+=to_string((int)liter_string[i]);
    }
    return int_list;
}

int CodeGenerator::NewString()
{
    return ++StringNumber;
}



int CodeGenerator::AddressGlobalVar(string identifier)
{
    int register_i=register_manager.Alloc();

    OutFile<<"\tmov\t"<<register_manager.Name(register_i)<<", "<<identifier<<endl;

    return register_i;
}

int CodeGenerator::AddressLocalVar(string identifier)
{
    int register_i=register_manager.Alloc();

    int local_variable_stack_offset=Local_Vartable().Visit(identifier).stack_offset;
    OutFile<<"\tlea\t"<<register_manager.Name(register_i)<<", [rbp+"<<local_variable_stack_offset<<"]"<<endl;

    return register_i;
}



int CodeGenerator::Dereference(int r_i,Type ptr_type)
{
    switch(ptr_type)
    {
        case T_CHAR_PTR:case T_CHAR_ARRAY:
            OutFile<<"\tmovzx\t"<<register_manager.Name(r_i)<<", byte ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
        case T_INT_PTR:case T_INT_ARRAY:
            OutFile<<"\tmovsx\t"<<register_manager.Name(r_i)<<", dword ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
        case T_LONG_PTR:case T_LONG_ARRAY:
            OutFile<<"\tmov\t"<<register_manager.Name(r_i)<<", ["<<register_manager.Name(r_i)<<"]"<<endl;
            break;
        default:
            CodeGenerate_Error("This type can't be dereferenced.");
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



int CodeGenerator::Copy(int r1_i,int r2_i)
{
    OutFile<<"\tmov\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;
    
    register_manager.Free(r2_i);
    return r1_i;
}



int CodeGenerator::Compare(int r1_i,int r2_i,string setx)
{
    OutFile<<"\tcmp\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;
    OutFile<<"\t"<<setx<<"\t"<<register_manager.Name(r1_i,1)<<endl;
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

int CodeGenerator::Mod(int r1_i,int r2_i)
{
    OutFile<<"\tmov\trax, "<<register_manager.Name(r1_i)<<endl;
    OutFile<<"\tcqo"<<endl;
    OutFile<<"\tidiv\t"<<register_manager.Name(r2_i)<<endl;
    OutFile<<"\tmov\t"<<register_manager.Name(r1_i)<<", rdx"<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::And(int r1_i,int r2_i)
{
    OutFile<<"\tand\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Or(int r1_i,int r2_i)
{
    OutFile<<"\tor\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Xor(int r1_i,int r2_i)
{
    OutFile<<"\txor\t"<<register_manager.Name(r1_i)<<", "<<register_manager.Name(r2_i)<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::Invert(int r_i)
{
    OutFile<<"\tnot\t"<<register_manager.Name(r_i)<<endl;
    
    return r_i;
}

int CodeGenerator::Not(int r_i)
{
    OutFile<<"\ttest\t"<<register_manager.Name(r_i)<<", "<<register_manager.Name(r_i)<<endl;
    OutFile<<"\tsete\t"<<register_manager.Name(r_i,1)<<endl;
    OutFile<<"\tmovzx\t"<<register_manager.Name(r_i)<<", "<<register_manager.Name(r_i,1)<<endl;
    
    return r_i;
}

int CodeGenerator::Negate(int r_i)
{
    OutFile<<"\tneg\t"<<register_manager.Name(r_i)<<endl;
    
    return r_i;
}



int CodeGenerator::ShiftLeftConstant(int r_i,int constant)
{
    OutFile<<"\tsal\t"<<register_manager.Name(r_i)<<", "<<constant<<endl;
    return r_i;
}

int CodeGenerator::ShiftLeft(int r1_i,int r2_i)
{
    OutFile<<"\tmov\tcl, "<<register_manager.Name(r2_i,1)<<endl;
    OutFile<<"\tshl\t"<<register_manager.Name(r1_i)<<", cl"<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}

int CodeGenerator::ShiftRight(int r1_i,int r2_i)
{
    OutFile<<"\tmov\tcl, "<<register_manager.Name(r2_i,1)<<endl;
    OutFile<<"\tshr\t"<<register_manager.Name(r1_i)<<", cl"<<endl;

    register_manager.Free(r2_i);
    return r1_i;
}



int CodeGenerator::IncGlobalVar(string identifier,string pre_post)
{
    int register_i=-1;

    if(pre_post=="post")register_i=LoadGlobalVar(identifier);

    switch(global_vartable.Visit(identifier).type)
    {
        case T_CHAR:
            OutFile<<"\tinc\tbyte ["<<identifier<<"]"<<endl;break;
        case T_INT:
            OutFile<<"\tinc\tdword ["<<identifier<<"]"<<endl;break;
        case T_LONG:
            OutFile<<"\tinc\tqword ["<<identifier<<"]"<<endl;break;
        default:
            CodeGenerate_Error("Can not inc this type.");break;
    }

    if(pre_post=="pre")register_i=LoadGlobalVar(identifier);

    return register_i;
}

int CodeGenerator::IncLocalVar(string identifier,string pre_post)
{
    int register_i=-1;

    if(pre_post=="post")register_i=LoadLocalVar(identifier);

    Type type=Local_Vartable().Visit(identifier).type;
    int local_variable_stack_offset=Local_Vartable().Visit(identifier).stack_offset;

    switch(type)
    {
        case T_CHAR:
            OutFile<<"\tinc\tbyte [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        case T_INT:
            OutFile<<"\tinc\tdword [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        case T_LONG:
            OutFile<<"\tinc\tqword [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        default:
            CodeGenerate_Error("Can not inc this type.");break;
    }

    if(pre_post=="pre")register_i=LoadLocalVar(identifier);

    return register_i;
}



int CodeGenerator::DecGlobalVar(string identifier,string pre_post)
{
    int register_i=-1;

    if(pre_post=="post")register_i=LoadGlobalVar(identifier);

    switch(global_vartable.Visit(identifier).type)
    {
        case T_CHAR:
            OutFile<<"\tdec\tbyte ["<<identifier<<"]"<<endl;break;
        case T_INT:
            OutFile<<"\tdec\tdword ["<<identifier<<"]"<<endl;break;
        case T_LONG:
            OutFile<<"\tdec\tqword ["<<identifier<<"]"<<endl;break;
        default:
            CodeGenerate_Error("Can not dec this type.");break;
    }

    if(pre_post=="pre")register_i=LoadGlobalVar(identifier);

    return register_i;
}

int CodeGenerator::DecLocalVar(string identifier,string pre_post)
{
    int register_i=-1;

    if(pre_post=="post")register_i=LoadLocalVar(identifier);

    Type type=Local_Vartable().Visit(identifier).type;
    int local_variable_stack_offset=Local_Vartable().Visit(identifier).stack_offset;
    
    switch(type)
    {
        case T_CHAR:
            OutFile<<"\tdec\tbyte [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        case T_INT:
            OutFile<<"\tdec\tdword [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        case T_LONG:
            OutFile<<"\tdec\tqword [rbp+"<<local_variable_stack_offset<<"]"<<endl;break;
        default:
            CodeGenerate_Error("Can not dec this type.");break;
    }

    if(pre_post=="pre")register_i=LoadLocalVar(identifier);

    return register_i;
}



void CodeGenerator::FunctionHead(string identifier)
{
    //OutFile<<"section\t.text"<<endl;
    OutFile<<"global\t"<<identifier<<endl;

    OutFile<<identifier<<":"<<endl;
    OutFile<<"\tpush\trbp"<<endl;
    OutFile<<"\tmov\trbp, rsp"<<endl;
    //very important
    if(Now_Function().stack_align_offset)OutFile<<"\tadd\trsp, "<<-Now_Function().stack_align_offset<<endl;
    LoadParameter();
}

void CodeGenerator::FunctionTail(string identifier)
{
    LablePrint(function_table.Visit(identifier).end_lable);
    //very important
    if(Now_Function().stack_align_offset)OutFile<<"\tadd\trsp, "<<Now_Function().stack_align_offset<<endl;

    OutFile<<"\tpop\trbp"<<endl;
    OutFile<<"\tret"<<endl<<endl<<endl;
}

int CodeGenerator::FunctionCall(string identifier)
{
//temporary storage
    vector<string>UsedRegister;
    for(int i=0;i<register_manager.General_Register_Table.size();i++)
    {
        if(!register_manager.General_Register_Table[i].free)
        {
            UsedRegister.push_back(register_manager.Name(i));
        }
    }

    for(int i=0;i<UsedRegister.size();i++)
    {
        OutFile<<"\tpush\t"<<UsedRegister[i]<<endl;
    }

    int out_ri=register_manager.Alloc();

    OutFile<<"\tcall\t"<<identifier<<endl;
    OutFile<<"\tmov\t"<<register_manager.Name(out_ri)<<", rax"<<endl;

//recover
    for(int i=UsedRegister.size()-1;i>=0;i--)
    {
        OutFile<<"\tpop\t"<<UsedRegister[i]<<endl;
    }

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

    if(function_table.Visit(identifier).type!=T_VOID)
        register_manager.Free(r_i);
}



void CodeGenerator::LoadParameter()
{
    for(int i=0;i<Now_Function().parameter_list.size();i++)
    {
        Type parameter_type=Now_Function().parameter_list[i].type;
        string parameter_identifier=Now_Function().parameter_list[i].identifier;
        int parameter_stack_offset=Local_Vartable().Visit(parameter_identifier).stack_offset;

        switch(parameter_type)
        {
            case T_CHAR:
                OutFile<<"\tmov\tbyte [rbp+"<<parameter_stack_offset<<"], "<<register_manager.Parameter_Register_Name(i,1)<<endl; 
                break;
            case T_INT:
                OutFile<<"\tmov\tdword [rbp+"<<parameter_stack_offset<<"], "<<register_manager.Parameter_Register_Name(i,4)<<endl; 
                break;
            case T_LONG:
            case T_CHAR_PTR: case T_INT_PTR: case T_LONG_PTR:
                OutFile<<"\tmov\tqword [rbp+"<<parameter_stack_offset<<"], "<<register_manager.Parameter_Register_Name(i,8)<<endl; 
                break;
            default:
                CodeGenerate_Error("Load "+parameter_identifier+" error.");
        }

    }
}

void CodeGenerator::StoreArgument(ASTNode* expression_node)
{
    int arguments_size=(expression_node->Children.size()+1)/2;
    
    for(int i=0,j=0;i<arguments_size;i++,j+=2)
    {
        int r_i=CodeGenerate_Assignment_Expression(expression_node->Children[j]);

        OutFile<<"\tmov\t"<<register_manager.Parameter_Register_Name(i)<<", "
               <<register_manager.Name(r_i)<<endl;
        
        register_manager.Free(r_i);
    }
}




//Tools
void CodeGenerator::Guarantee_Exist_GlobalVartable(string identifier,ASTNode* error_node)
{
    if(!global_vartable.Exist(identifier))
    {
        CodeGenerate_Error("The variable "+identifier+" is not defined.",error_node);
    }
}

void CodeGenerator::Guarantee_InExist_GlobalVartable(string identifier,ASTNode* error_node)
{
    if(global_vartable.Exist(identifier))
    {
        CodeGenerate_Error("The variable "+identifier+" is redefined.",error_node);
    }
}

void CodeGenerator::Guarantee_Exist_LocalVartable(string identifier,ASTNode* error_node)
{
    if(!Local_Vartable().Exist(identifier))
    {
        CodeGenerate_Error("The variable "+identifier+" is not defined.",error_node);
    }
}

void CodeGenerator::Guarantee_InExist_LocalVartable(string identifier,ASTNode* error_node)
{
    if(Local_Vartable().Exist(identifier))
    {
        CodeGenerate_Error("The variable "+identifier+" is redefined.",error_node);
    }
}



VariableTable& CodeGenerator::Local_Vartable()
{
    return Now_Function().local_vartable;
}

Function& CodeGenerator::Now_Function()
{
    return function_table.Visit(NowInFunction);
}



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

Type CodeGenerator::Type_To_ArrayType(Type type)
{
    Type array_type;

    switch(type)
    {
        case T_CHAR:array_type=T_CHAR_ARRAY;break;
        case T_INT: array_type=T_INT_ARRAY;break;
        case T_LONG:array_type=T_LONG_ARRAY;break;
    }

    return array_type;
}



int CodeGenerator::Address_ScaleFactor(Type type)
{
    int factor=0;

    switch(type)
    {
        case T_CHAR:    case T_CHAR_ARRAY:  factor=0;break;
        case T_INT:     case T_INT_ARRAY:   factor=2;break;
        case T_LONG:    case T_LONG_ARRAY:  factor=3;break;
    }

    return factor;
}

int CodeGenerator::Dreference_ScaleFactor(Type type)
{
    int factor=0;

    switch(type)
    {   
        case T_CHAR_PTR:    case T_CHAR_ARRAY:  factor=0;break;
        case T_INT_PTR:     case T_INT_ARRAY:   factor=2;break;
        case T_LONG_PTR:    case T_LONG_ARRAY:  factor=3;break;
    }

    return factor;
}



void CodeGenerator::Calculate_StackOffset(ASTNode* compound_node)
{
    for(int i=1;i<compound_node->Children.size()-1;i++)
    {
        ASTNode* node=FirstChild(compound_node->Children[i]);
        if(node->type==LOCALVARIABLE_DEFINITION)
        {
            //BUG
            if(node->Children[0]->Children[0]->Children[0]->type==AST_CHAR||node->Children[0]->Children[0]->Children[0]->type==AST_INT)
            {
                if(node->Children[0]->Children[1]->type==AST_STAR)
                {
                    Now_Function().stack_align_offset+=8;
                }
                else Now_Function().stack_align_offset+=4;
            }
            else if(node->Children[0]->Children[0]->Children[0]->type==AST_LONG)
            {
                Now_Function().stack_align_offset+=8;
            }
        }
    }
    Now_Function().stack_align_offset=(Now_Function().stack_align_offset+15) & ~15;
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

void CodeGenerator::CodeGenerate_Error(string error_message,ASTNode* error_node)
{
    cout<< "CodeGenerate Error: Line "<<error_node->line<<": "<<error_message<<endl;
    exit(5);
}
