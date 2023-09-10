#include <iostream>

#include "Parser.h"
#include "Token.h"
#include "AbstractSyntaxTree.h"
#include "Interpreter.h"

using namespace std;

void Parser::Parse()
{
    ast.Translation=Translation_Unit();

    cout<<endl<<"----- The AST -----"<<endl;
    ast.AST_Print();
    cout<<endl<<"----- Print Done -----"<<endl;

    //Interpreter I;
    //I.ast=this->ast;
    //I.Interpret();

}



ASTNode* Parser::Translation_Unit()
{
    WhoAmI("Translation_Unit");

    ASTNode* node=new ASTNode(TRANSLATION_UNIT);

    while(!Is_AtEnd())Add_Child(node,Function_Definition());
    
    return node;    
}



ASTNode* Parser::Function_Definition()
{
    WhoAmI("Function_Definition");

    ASTNode * node=new ASTNode(FUNCTION_DEFINITION);
    
    Add_Child(node,Function_Type());

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Compound_Statement());
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Identifier loss.");
    
    return node;
}

ASTNode* Parser::Function_Type()
{
    WhoAmI("Function_Type");

    ASTNode* node=new ASTNode(FUNCTION_TYPE);

    if(Match(VOID))Add_Child(node,new ASTNode(AST_VOID,Previous_Token()));
    else if(Match(INT))Add_Child(node,new ASTNode(AST_INT,Previous_Token()));
    else if(Match(CHAR))Add_Child(node,new ASTNode(AST_CHAR,Previous_Token()));
    else if(Match(LONG))Add_Child(node,new ASTNode(AST_LONG,Previous_Token()));
    else Parse_Error("Function type loss.");

    return node;
}





ASTNode* Parser::Statement()
{
    WhoAmI("Statement");

    ASTNode* node=new ASTNode(STATEMENT);

    if(Peek(PRINT))Add_Child(node,Print_Statement());
    else if(Peek_VariableType())Add_Child(node,Variable_Definition());
    else if(Peek(LEFT_BRACE))Add_Child(node,Compound_Statement());
    else if(Peek(IF))Add_Child(node,If_Statement());
    else if(Peek(WHILE)||Peek(DO))Add_Child(node,Iteration_Statement());
    else if(Peek(RETURN))Add_Child(node,Return_Statement());
    else Add_Child(node,Expression_Statement());

    return node;
}

ASTNode* Parser::Compound_Statement()
{
    WhoAmI("Compound_Statement");

    ASTNode* node=new ASTNode(COMPOUND_STATEMENT);
    
    if (Match(LEFT_BRACE))
	{
        Add_Child(node,new ASTNode(AST_LEFT_BARCE,Previous_Token()));
        while(!Match(RIGHT_BRACE))Add_Child(node,Statement());
        
        if(Previous_Token().type!=RIGHT_BRACE)Parse_Error("Right barce } loss.");
        Add_Child(node,new ASTNode(AST_RIGHT_BRACE,Previous_Token()));
	}
	else Parse_Error("Left barce { loss.");

    return node;
}



ASTNode* Parser::Print_Statement()
{
    WhoAmI("Print_Statement");

    ASTNode* node=new ASTNode(PRINT_STATEMENT);

    if(Match(PRINT))
    {
        Add_Child(node,new ASTNode(AST_PRINT,Previous_Token()));
        Add_Child(node,Expression());

        Match_Semicolon(node);
    }
    else Parse_Error("Keyword print loss.");

    return node;
}



ASTNode* Parser::Variable_Definition()
{
    WhoAmI("Variable_Definition");

    ASTNode* node=new ASTNode(VARIABLE_DEFINITION);

    Add_Child(node,Variable_Declaration());

    if(Match(ASSIGN))
    {
        Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
        Add_Child(node,Expression());
    }  

    Match_Semicolon(node);

    return node;
}

ASTNode* Parser::Variable_Declaration()
{   
    WhoAmI("Variable_Declaration");

    ASTNode* node=new ASTNode(VARIABLE_DECLARATION);

    Add_Child(node,Variable_Type());

    if(Match(ID))Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
    else Parse_Error("Identifier loss.");

    return node;
}

ASTNode* Parser::Variable_Type()
{
    WhoAmI("Variable_Type");

    ASTNode* node=new ASTNode(VARIABLE_TYPE);

    if(Match(INT))Add_Child(node,new ASTNode(AST_INT,Previous_Token()));
    else if(Match(CHAR))Add_Child(node,new ASTNode(AST_CHAR,Previous_Token()));
    else if(Match(LONG))Add_Child(node,new ASTNode(AST_LONG,Previous_Token()));
    else Parse_Error("Variable type loss.");

    return node;
}



ASTNode* Parser::If_Statement()
{
    WhoAmI("If_Statement");
    
    ASTNode* node=new ASTNode(IF_STATEMENT);

    if(Match(IF))
    {
        Add_Child(node,new ASTNode(AST_IF,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Expression());
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Statement());
                if(Match(ELSE))
                {   
                    Add_Child(node,new ASTNode(AST_ELSE,Previous_Token()));
                    Add_Child(node,Statement());
                }
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Keyword if loss.");

    return node;
}



ASTNode* Parser::Iteration_Statement()
{
    WhoAmI("Iteration_Statement");

    ASTNode* node=new ASTNode(ITERATION_STATEMENT);

    if(Peek(WHILE))Add_Child(node,While_Statement());
    else if(Peek(DO))Add_Child(node,DoWhile_Statement());
    else Parse_Error("Wrong loop statement.");

    return node;
}

ASTNode* Parser::While_Statement()
{
    WhoAmI("While_Statement");
    
    ASTNode* node=new ASTNode(WHILE_STATEMENT);

    if(Match(WHILE))
    {
        Add_Child(node,new ASTNode(AST_WHILE,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Expression());
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Statement());
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Keyword while loss.");

    return node;
}
    
ASTNode* Parser::DoWhile_Statement()
{
    WhoAmI("DoWhile_Statement");

    ASTNode* node=new ASTNode(DOWHILE_STATEMENT);

    if(Match(DO))
    {
        Add_Child(node,new ASTNode(AST_DO,Previous_Token()));
        Add_Child(node,Statement());
        if(Match(WHILE))
        {
            Add_Child(node,new ASTNode(AST_WHILE,Previous_Token()));
            if(Match(LEFT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
                Add_Child(node,Expression());
                if(Match(RIGHT_PAREN))
                {
                    Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                    
                    Match_Semicolon(node);
                }
                else Parse_Error("Right paren ) loss.");
            }
            else Parse_Error("Left paren ( loss.");
        }
        else Parse_Error("Keyword while loss.");
    }
    else Parse_Error("Keyword do loss.");

    return node;
}



ASTNode* Parser::Expression_Statement()
{
    WhoAmI("Expression_Statement");

    ASTNode* node=new ASTNode(EXPRESSION_STATEMENT);

    if(!Peek(SEMICOLON))Add_Child(node,Expression());
    Match_Semicolon(node);
    
    return node;
}



ASTNode* Parser::Return_Statement()
{
    WhoAmI("Return_Statement");

    ASTNode* node=new ASTNode(RETURN_STATEMENT);

    if(Match(RETURN))
    {
        Add_Child(node,new ASTNode(AST_RETURN,Previous_Token()));
        
        if(!Peek(SEMICOLON))Add_Child(node,Expression());
        Match_Semicolon(node);
    }
    else Parse_Error("Keyword return loss.");

    return node;
}





ASTNode* Parser::Expression()
{
    WhoAmI("Expression");

    ASTNode* node=new ASTNode(EXPRESSION);

    Add_Child(node,Assignment_Expression());

    while(Match(COMMA))
    {
        Add_Child(node,new ASTNode(AST_COMMA,Previous_Token()));
        Add_Child(node,Assignment_Expression());
    }
    
    return node;
}

ASTNode* Parser::Assignment_Expression()
{
    WhoAmI("Assignment_Expression");

    ASTNode* node=new ASTNode(ASSIGNMENT_EXPRESSION);

    if(Peek(ID)&&Peek(ASSIGN,2))
    {
        Match(ID);Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        Match(ASSIGN);Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
    }

   Add_Child(node,Equality_Expression());

   return node; 
}



ASTNode* Parser::Equality_Expression()
{
    WhoAmI("Equality_Expression");

    ASTNode* node=new ASTNode(EQUALITY_EXPRESSION);

    Add_Child(node,Relational_Expression());

    while(Match(EQUAL)||Match(NOT_EQUAL))
    {
        if(Previous_Token().type==EQUAL)
            Add_Child(node,new ASTNode(AST_EQUAL,Previous_Token()));
        else if(Previous_Token().type==NOT_EQUAL)
            Add_Child(node,new ASTNode(AST_NOT_EQUAL,Previous_Token()));
         
        Add_Child(node,Relational_Expression()); 
    }

    return node;
}

ASTNode* Parser::Relational_Expression()
{
    WhoAmI("Relational_Expression");

    ASTNode* node=new ASTNode(RELATIONAL_EXPRESSION);

    Add_Child(node,PlusMinus_Expression());

    while(Match(LESS)||Match(LESS_EQUAL)||Match(GREATER)||Match(GREATER_EQUAL))
    {
        switch(Previous_Token().type)
        {
            case LESS:
                Add_Child(node,new ASTNode(AST_LESS,Previous_Token()));break;
            case LESS_EQUAL:
                Add_Child(node,new ASTNode(AST_LESS_EQUAL,Previous_Token()));break;
            case GREATER:
                Add_Child(node,new ASTNode(AST_GREATER,Previous_Token()));break;
            case GREATER_EQUAL:
                Add_Child(node,new ASTNode(AST_GREATER_EQUAL,Previous_Token()));break;  
        }
        
        Add_Child(node,PlusMinus_Expression());
    }

    return node;
}

ASTNode* Parser::PlusMinus_Expression()
{
    WhoAmI("PlusMinus_Expression");

    ASTNode* node=new ASTNode(PLUSMINUS_EXPRESSION);

    Add_Child(node,MulDiv_Expression());
   
    while(Match(PLUS)||Match(MINUS))
    {
        if(Previous_Token().type==PLUS)
        {
            Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
        }
        else if(Previous_Token().type==MINUS)Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));

        Add_Child(node,MulDiv_Expression());
    }

    return node;
}

ASTNode* Parser::MulDiv_Expression()
{
    WhoAmI("MulDiv_Expression");

    ASTNode* node= new ASTNode(MULDIV_EXPRESSION);

    Add_Child(node,Unary_Expression());

    while(Match(STAR)||Match(SLASH))
    {
        if(Previous_Token().type==STAR)
        {
            Add_Child(node,new ASTNode(AST_STAR,Previous_Token()));
        }
        else  if(Previous_Token().type==SLASH)Add_Child(node,new ASTNode(AST_SLASH,Previous_Token()));

        Add_Child(node,Unary_Expression());
    }

    return node;
}

ASTNode* Parser::Unary_Expression()
{
    WhoAmI("Unary_Expression");

    ASTNode* node=new ASTNode(UNARY_EXPRESSION);

    // if(Match(PLUS)||Match(MINUS))
    // {   
    //     if(Previous_Token().type==PLUS)
    //     {
    //         Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
    //     }
    //     else if(Previous_Token().type==MINUS)Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));

    //     Add_Child(node,Unary_Expression());
    // }
    // else 
    Add_Child(node,Primary_Expression());
    
    return node;
}

ASTNode* Parser::Primary_Expression()
{
    WhoAmI("Primary_Expression");

    ASTNode* node=new ASTNode(PRIMARY_EXPRESSION);

    if(Match(CONSTANT_INT))Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
    else if(Peek(ID))
    {
        if(Peek(LEFT_PAREN,2))Add_Child(node,FunctionCall_Expression());
        else 
        {
            Match(ID);
            Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        }
    }
    else Parse_Error("Primary character loss.");

    return node;
}

ASTNode* Parser::FunctionCall_Expression()
{
    WhoAmI("FunctionCall_Expression");

    ASTNode* node=new ASTNode(FUNCTIONCALL_EXPRESSION);

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Expression());
            if(Match(RIGHT_PAREN))Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Identifier loss.");

    return node;
}



void Parser::Parse_Error(string error_message)
{
    cout<< "Parse Error: Line "<<tokens[current].line<<": "<<error_message<<endl;
    exit(2);
}



bool Parser::Is_AtEnd()
{
    return tokens[current].type==CODE_EOF;
}

bool Parser::Match(TokenType expected)
{
    if(!Is_AtEnd()&&expected==tokens[current].type)
    {
        current++;
        return true;
    }
    return false;
}

bool Parser::Peek(TokenType expected)
{
    return Peek(expected,1);
}

bool Parser::Peek(TokenType expected,int n)
{
    if(current+n-1<tokens.size()-1&&expected==tokens[current+n-1].type)return true;
    return false;
}

bool Parser::Peek_VariableType()
{
    return Peek(INT)||Peek(CHAR)||Peek(LONG);
}

Token Parser::Previous_Token()
{
    return tokens[current-1];
}

void Parser::Add_Child(ASTNode* root,ASTNode* child)
{
    if(root)root->Children.push_back(child);
    else Parse_Error("Access null ASTNode pointer");
}

void Parser::Match_Semicolon(ASTNode* root)
{
    if(Match(SEMICOLON))Add_Child(root,new ASTNode(AST_SEMICOLON,Previous_Token()));        
    else Parse_Error("Semicolon ; loss.");
}



void Parser::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}