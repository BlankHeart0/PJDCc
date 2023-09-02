#include <iostream>

#include "Parser.h"
#include "Token.h"
#include "AbstractSyntaxTree.h"
#include "Interpreter.h"

using namespace std;

void Parser::Parse()
{
    A.Translation=Translation_Unit();

    if(!is_error)
    {
        cout<<endl<<"----- The AST -----"<<endl;
        A.AST_Print();
        cout<<endl<<"----- Print Done -----"<<endl;

        Interpreter I;
        I.A=this->A;
        I.Interpret();
    }
}



ASTNode* Parser::Translation_Unit()
{
    WhoAmI("Translation_Unit");

    ASTNode* node=new ASTNode(false,TRANSLATION_UNIT,0);

    node->Children.push_back(Expression());
    
    return node;    
}

ASTNode* Parser::Expression()
{
    WhoAmI("Expression");

    ASTNode* node=new ASTNode(false,EXPRESSION,0);

    node->Children.push_back(PlusMinus_Expression());
    
    return node;
}

ASTNode* Parser::PlusMinus_Expression()
{
    WhoAmI("PlusMinus_Expression");

    ASTNode* node=new ASTNode(false,PLUSMINUS_EXPRESSION,0);

    node->Children.push_back(MulDiv_Expression());
   
    while(!is_error&&(Match(PLUS)||Match(MINUS)))
    {
        if(Previous_TokenType()==PLUS)
        {
            node->Children.push_back(new ASTNode(true,AST_PLUS,0));
        }
        else if(Previous_TokenType()==MINUS)node->Children.push_back(new ASTNode(true,AST_MINUS,0));

        node->Children.push_back(MulDiv_Expression());
    }

    return node;
}

ASTNode* Parser::MulDiv_Expression()
{
    WhoAmI("MulDiv_Expression");

    ASTNode* node= new ASTNode(false,MULDIV_EXPRESSION,0);

    node->Children.push_back(Unary_Expression());

    while(!is_error&&(Match(STAR)||Match(SLASH)))
    {
        if(Previous_TokenType()==STAR)
        {
            node->Children.push_back(new ASTNode(true,AST_STAR,0));
        }
        else  if(Previous_TokenType()==SLASH)node->Children.push_back(new ASTNode(true,AST_SLASH,0));

        node->Children.push_back(Unary_Expression());
    }

    return node;
}

ASTNode* Parser::Unary_Expression()
{
    WhoAmI("Unary_Expression");

    ASTNode* node=new ASTNode(false,UNARY_EXPRESSION,0);

    if(Match(PLUS)||Match(MINUS))
    {   
        if(Previous_TokenType()==PLUS)
        {
            node->Children.push_back(new ASTNode(true,AST_PLUS,0));
        }
        else if(Previous_TokenType()==MINUS)node->Children.push_back(new ASTNode(true,AST_MINUS,0));

        node->Children.push_back(Unary_Expression());
    }
    else node->Children.push_back(Primary_Expression());
    
    return node;
}

ASTNode* Parser::Primary_Expression()
{
    WhoAmI("Primary_Expression");

    ASTNode* node=new ASTNode(false,PRIMARY_EXPRESSION,0);

    if(Match(CONSTANT_INT))
    {        
        node->Children.push_back(new ASTNode(true,AST_CONSTANT_INT,tokens[current-1].literal));
    }
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

TokenType Parser::Previous_TokenType()
{
    return tokens[current-1].type;
}

void Parser::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}