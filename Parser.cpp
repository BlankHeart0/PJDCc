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

    while(!Is_AtEnd())Add_Child(node,Statement());
    
    return node;    
}



ASTNode* Parser::Statement()
{
    WhoAmI("Statement");

    ASTNode* node=new ASTNode(STATEMENT);

    if(Peek(PRINT))Add_Child(node,Print_Statement());
    else if(Peek(ID))Add_Child(node,Assignment_Statement());
    else if(Peek(LEFT_BRACE))Add_Child(node,Compound_Statement());
    else if(Peek(IF))Add_Child(node,If_Statement());
    else Add_Child(node,Variable_Definition());

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

        if(Match(SEMICOLON))Add_Child(node,new ASTNode(AST_SEMICOLON,Previous_Token()));        
        else Parse_Error("Semicolon ; loss.");
    }
    else Parse_Error("Keyword print loss.");

    return node;
}

ASTNode* Parser::Assignment_Statement()
{
    WhoAmI("Assignment_Statement");

    ASTNode* node=new ASTNode(ASSIGNMENT_STATEMENT);

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(ASSIGN))
        {
            Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
            Add_Child(node,Expression());

            if(Match(SEMICOLON))Add_Child(node,new ASTNode(AST_SEMICOLON,Previous_Token()));        
            else Parse_Error("Semicolon ; loss.");
        }
        else Parse_Error("Assign character = loss.");
    }
    else Parse_Error("Identifier loss.");

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

    if(Match(SEMICOLON))Add_Child(node,new ASTNode(AST_SEMICOLON,Previous_Token()));        
    else Parse_Error("Semicolon ; loss.");

    return node;
}

ASTNode* Parser::Variable_Declaration()
{   
    WhoAmI("Variable_Declaration");

    ASTNode* node=new ASTNode(VARIABLE_DECLARATION);

    if(Match(INT))
    {
        Add_Child(node,new ASTNode(AST_INT,Previous_Token()));
        if(Match(ID))
        {
            Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        }
        else Parse_Error("Identifier loss.");
    }
    else Parse_Error("Keyword int loss.");

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



ASTNode* Parser::Expression()
{
    WhoAmI("Expression");

    ASTNode* node=new ASTNode(EXPRESSION);

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

    if(Match(PLUS)||Match(MINUS))
    {   
        if(Previous_Token().type==PLUS)
        {
            Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
        }
        else if(Previous_Token().type==MINUS)Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));

        Add_Child(node,Unary_Expression());
    }
    else Add_Child(node,Primary_Expression());
    
    return node;
}

ASTNode* Parser::Primary_Expression()
{
    WhoAmI("Primary_Expression");

    ASTNode* node=new ASTNode(PRIMARY_EXPRESSION);

    if(Match(CONSTANT_INT))Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
    else if(Match(ID))Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
    else Parse_Error("Primary character loss.");

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
    if(!Is_AtEnd()&&expected==tokens[current].type)return true;
    return false;
}

Token Parser::Previous_Token()
{
    return tokens[current-1];
}

void Parser::Add_Child(ASTNode* root,ASTNode* child)
{
    root->Children.push_back(child);
}

void Parser::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}