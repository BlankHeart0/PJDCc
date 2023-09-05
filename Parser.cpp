#include <iostream>

#include "Parser.h"
#include "Token.h"
#include "AbstractSyntaxTree.h"
#include "Interpreter.h"

using namespace std;

void Parser::Parse()
{
    ast.Translation=Translation_Unit();

    if(!is_error)
    {
        cout<<endl<<"----- The AST -----"<<endl;
        ast.AST_Print();
        cout<<endl<<"----- Print Done -----"<<endl;

        //Interpreter I;
        //I.ast=this->ast;
        //I.Interpret();
    }
}



ASTNode* Parser::Translation_Unit()
{
    WhoAmI("Translation_Unit");

    ASTNode* node=new ASTNode(TRANSLATION_UNIT);

    while(!is_error&&!Is_AtEnd())node->Children.push_back(Statement());
    
    return node;    
}



ASTNode* Parser::Statement()
{
    WhoAmI("Statement");

    ASTNode* node=new ASTNode(STATEMENT);

    if(Peek(PRINT))node->Children.push_back(Print_Statement());
    else if(Peek(ID))node->Children.push_back(Assignment_Statement());
    else node->Children.push_back(Variable_Definition());

    return node;
}

ASTNode* Parser::Print_Statement()
{
    WhoAmI("Print_Statement");

    ASTNode* node=new ASTNode(PRINT_STATEMENT);

    if(Match(PRINT))
    {
        node->Children.push_back(new ASTNode(AST_PRINT,Previous_Token()));
        node->Children.push_back(Expression());
        if(is_error)return nullptr;

        if(Match(SEMICOLON))node->Children.push_back(new ASTNode(AST_SEMICOLON,Previous_Token()));        
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
        node->Children.push_back(new ASTNode(AST_ID,Previous_Token()));
        if(Match(ASSIGN))
        {
            node->Children.push_back(new ASTNode(AST_ASSIGN,Previous_Token()));
            node->Children.push_back(Expression());
            if(is_error)return nullptr;

            if(Match(SEMICOLON))node->Children.push_back(new ASTNode(AST_SEMICOLON,Previous_Token()));        
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

    node->Children.push_back(Variable_Declaration());
    if(is_error)return nullptr;

    if(Match(ASSIGN))
    {
        node->Children.push_back(new ASTNode(AST_ASSIGN,Previous_Token()));
        node->Children.push_back(Expression());
        if(is_error)return nullptr;
    }  

    if(Match(SEMICOLON))node->Children.push_back(new ASTNode(AST_SEMICOLON,Previous_Token()));        
    else Parse_Error("Semicolon ; loss.");

    return node;
}

ASTNode* Parser::Variable_Declaration()
{   
    WhoAmI("Variable_Declaration");

    ASTNode* node=new ASTNode(VARIABLE_DECLARATION);

    if(Match(INT))
    {
        node->Children.push_back(new ASTNode(AST_INT,Previous_Token()));
        if(Match(ID))
        {
            node->Children.push_back(new ASTNode(AST_ID,Previous_Token()));
        }
        else Parse_Error("Identifier loss.");
    }
    else Parse_Error("Keyword int loss.");

    return node;
}



ASTNode* Parser::Expression()
{
    WhoAmI("Expression");

    ASTNode* node=new ASTNode(EXPRESSION);

    node->Children.push_back(PlusMinus_Expression());
    
    return node;
}

ASTNode* Parser::PlusMinus_Expression()
{
    WhoAmI("PlusMinus_Expression");

    ASTNode* node=new ASTNode(PLUSMINUS_EXPRESSION);

    node->Children.push_back(MulDiv_Expression());
   
    while(!is_error&&(Match(PLUS)||Match(MINUS)))
    {
        if(Previous_Token().type==PLUS)
        {
            node->Children.push_back(new ASTNode(AST_PLUS,Previous_Token()));
        }
        else if(Previous_Token().type==MINUS)node->Children.push_back(new ASTNode(AST_MINUS,Previous_Token()));

        node->Children.push_back(MulDiv_Expression());
    }

    return node;
}

ASTNode* Parser::MulDiv_Expression()
{
    WhoAmI("MulDiv_Expression");

    ASTNode* node= new ASTNode(MULDIV_EXPRESSION);

    node->Children.push_back(Unary_Expression());

    while(!is_error&&(Match(STAR)||Match(SLASH)))
    {
        if(Previous_Token().type==STAR)
        {
            node->Children.push_back(new ASTNode(AST_STAR,Previous_Token()));
        }
        else  if(Previous_Token().type==SLASH)node->Children.push_back(new ASTNode(AST_SLASH,Previous_Token()));

        node->Children.push_back(Unary_Expression());
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
            node->Children.push_back(new ASTNode(AST_PLUS,Previous_Token()));
        }
        else if(Previous_Token().type==MINUS)node->Children.push_back(new ASTNode(AST_MINUS,Previous_Token()));

        node->Children.push_back(Unary_Expression());
    }
    else node->Children.push_back(Primary_Expression());
    
    return node;
}

ASTNode* Parser::Primary_Expression()
{
    WhoAmI("Primary_Expression");

    ASTNode* node=new ASTNode(PRIMARY_EXPRESSION);

    if(Match(CONSTANT_INT))node->Children.push_back(new ASTNode(AST_CONSTANT_INT,Previous_Token()));
    else if(Match(ID))node->Children.push_back(new ASTNode(AST_ID,Previous_Token()));
    else Parse_Error("Primary character loss.");

    return node;
}



void Parser::Parse_Error(string error_message)
{
    is_error=true;
    cout<< "Parse Error: Line "<<tokens[current].line<<": "<<error_message<<endl;
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

void Parser::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}