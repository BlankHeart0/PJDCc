#include <iostream>

#include "Parser.h"
#include "Token.h"
#include "AbstractSyntaxTree.h"

using namespace std;

void Parser::Parse()
{
    ast.Translation=Parse_Translation_Unit();
}



// Begin
ASTNode* Parser::Parse_Translation_Unit()
{
    WhoAmI("Parse_Translation_Unit");

    ASTNode* node=new ASTNode(TRANSLATION_UNIT);

    while(!Is_AtEnd())
    {
        if(Peek(LEFT_PAREN,3))Add_Child(node,Parse_Function_Definition());
        else if(Peek(LEFT_SQUARE,3))Add_Child(node,Parse_Array_Definition());
        else Add_Child(node,Parse_Variable_Definition());
    }
    return node;    
}



// Definition, Declaration
ASTNode* Parser::Parse_Type()
{
    WhoAmI("Parse_Type");

    ASTNode* node=new ASTNode(TYPE);

    if(Match(VOID))Add_Child(node,new ASTNode(AST_VOID,Previous_Token()));
    else if(Match(CHAR))Add_Child(node,new ASTNode(AST_CHAR,Previous_Token()));
    else if(Match(INT))Add_Child(node,new ASTNode(AST_INT,Previous_Token()));
    else if(Match(LONG))Add_Child(node,new ASTNode(AST_LONG,Previous_Token()));
    
    else Parse_Error("Type loss.");

    return node;
}

ASTNode* Parser::Parse_Function_Definition()
{
    WhoAmI("Parse_Function_Definition");

    ASTNode * node=new ASTNode(FUNCTION_DEFINITION);
    
    Add_Child(node,Parse_Type());

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Parse_Compound_Statement());
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Function identifier loss.");
    
    return node;
}



ASTNode* Parser::Parse_Variable_Definition()
{
    WhoAmI("Parse_Variable_Definition");

    ASTNode* node=new ASTNode(VARIABLE_DEFINITION);

    Add_Child(node,Parse_Variable_Declaration());

    if(Match(ASSIGN))
    {
        Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
        Add_Child(node,Parse_Expression());
    }  

    Match_Semicolon(node);

    return node;
}

ASTNode* Parser::Parse_Variable_Declaration()
{   
    WhoAmI("Parse_Variable_Declaration");

    ASTNode* node=new ASTNode(VARIABLE_DECLARATION);

    Add_Child(node,Parse_Type());

    if(Match(STAR))Add_Child(node,new ASTNode(AST_STAR,Previous_Token()));

    if(Match(ID))Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
    else Parse_Error("Variable identifier loss.");

    return node;
}



ASTNode* Parser::Parse_Array_Definition()
{
    WhoAmI("Parse_Array_Definition");

    ASTNode* node=new ASTNode(ARRAY_DEFINITION);

    Add_Child(node,Parse_Type());

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_SQUARE))
        {
            Add_Child(node,new ASTNode(AST_LEFT_SQUARE,Previous_Token()));
            if(Match(CONSTANT_INT))
            {
                Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
                if(Match(RIGHT_SQUARE))
                {
                    Add_Child(node,new ASTNode(AST_RIGHT_SQUARE,Previous_Token()));
                    Match_Semicolon(node);
                }
                else Parse_Error("Right square ] loss.");
            }
            else Parse_Error("Array size loss.");
        }
        else Parse_Error("Left square [ loss.");
    }
    else Parse_Error("Array identifier loss.");

    return node;
}



// Statement
ASTNode* Parser::Parse_Statement()
{
    WhoAmI("Parse_Statement");

    ASTNode* node=new ASTNode(STATEMENT);

    if(Peek(LEFT_BRACE))Add_Child(node,Parse_Compound_Statement());
    else if(Peek_Type())Add_Child(node,Parse_Variable_Definition());
    else if(Peek(IF))Add_Child(node,Parse_If_Statement());
    else if(Peek(WHILE)||Peek(DO))Add_Child(node,Parse_Iteration_Statement());
    else if(Peek(RETURN))Add_Child(node,Parse_Return_Statement());
    else Add_Child(node,Parse_Expression_Statement());

    return node;
}



ASTNode* Parser::Parse_Compound_Statement()
{
    WhoAmI("Parse_Compound_Statement");

    ASTNode* node=new ASTNode(COMPOUND_STATEMENT);
    
    if (Match(LEFT_BRACE))
	{
        Add_Child(node,new ASTNode(AST_LEFT_BARCE,Previous_Token()));
        while(!Match(RIGHT_BRACE))Add_Child(node,Parse_Statement());
        
        if(Previous_Token().type!=RIGHT_BRACE)Parse_Error("Right barce } loss.");
        Add_Child(node,new ASTNode(AST_RIGHT_BRACE,Previous_Token()));
	}
	else Parse_Error("Left barce { loss.");

    return node;
}



ASTNode* Parser::Parse_If_Statement()
{
    WhoAmI("Parse_If_Statement");
    
    ASTNode* node=new ASTNode(IF_STATEMENT);

    if(Match(IF))
    {
        Add_Child(node,new ASTNode(AST_IF,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Parse_Expression());
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Parse_Statement());
                if(Match(ELSE))
                {   
                    Add_Child(node,new ASTNode(AST_ELSE,Previous_Token()));
                    Add_Child(node,Parse_Statement());
                }
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Keyword if loss.");

    return node;
}



ASTNode* Parser::Parse_Iteration_Statement()
{
    WhoAmI("Parse_Iteration_Statement");

    ASTNode* node=new ASTNode(ITERATION_STATEMENT);

    if(Peek(WHILE))Add_Child(node,Parse_While_Statement());
    else if(Peek(DO))Add_Child(node,Parse_DoWhile_Statement());
    else Parse_Error("Wrong loop statement.");

    return node;
}

ASTNode* Parser::Parse_While_Statement()
{
    WhoAmI("Parse_While_Statement");
    
    ASTNode* node=new ASTNode(WHILE_STATEMENT);

    if(Match(WHILE))
    {
        Add_Child(node,new ASTNode(AST_WHILE,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Parse_Expression());
            if(Match(RIGHT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                Add_Child(node,Parse_Statement());
            }
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Keyword while loss.");

    return node;
}
    
ASTNode* Parser::Parse_DoWhile_Statement()
{
    WhoAmI("Parse_DoWhile_Statement");

    ASTNode* node=new ASTNode(DOWHILE_STATEMENT);

    if(Match(DO))
    {
        Add_Child(node,new ASTNode(AST_DO,Previous_Token()));
        Add_Child(node,Parse_Statement());
        if(Match(WHILE))
        {
            Add_Child(node,new ASTNode(AST_WHILE,Previous_Token()));
            if(Match(LEFT_PAREN))
            {
                Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
                Add_Child(node,Parse_Expression());
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



ASTNode* Parser::Parse_Return_Statement()
{
    WhoAmI("Parse_Return_Statement");

    ASTNode* node=new ASTNode(RETURN_STATEMENT);

    if(Match(RETURN))
    {
        Add_Child(node,new ASTNode(AST_RETURN,Previous_Token()));
        
        if(!Peek(SEMICOLON))Add_Child(node,Parse_Expression());
        Match_Semicolon(node);
    }
    else Parse_Error("Keyword return loss.");

    return node;
}



ASTNode* Parser::Parse_Expression_Statement()
{
    WhoAmI("Parse_Expression_Statement");

    ASTNode* node=new ASTNode(EXPRESSION_STATEMENT);

    if(!Peek(SEMICOLON))Add_Child(node,Parse_Expression());
    Match_Semicolon(node);
    
    return node;
}



// Expression
ASTNode* Parser::Parse_Expression()
{
    WhoAmI("Parse_Expression");

    ASTNode* node=new ASTNode(EXPRESSION);

    Add_Child(node,Parse_Assignment_Expression());

    while(Match(COMMA))
    {
        Add_Child(node,new ASTNode(AST_COMMA,Previous_Token()));
        Add_Child(node,Parse_Assignment_Expression());
    }
    
    return node;
}

ASTNode* Parser::Parse_Assignment_Expression()
{
    WhoAmI("Parse_Assignment_Expression");

    ASTNode* node=new ASTNode(ASSIGNMENT_EXPRESSION);

    if(Peek(ID)&&Peek(ASSIGN,2))
    {
        Match(ID);Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        Match(ASSIGN);Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
    }

    else if(Peek(STAR)&&Peek(ID,2)&&Peek(ASSIGN,3))
    {
        Match(STAR);Add_Child(node,new ASTNode(AST_STAR,Previous_Token()));
        Match(ID);Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        Match(ASSIGN);Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
    }

    else if(Peek(ID)&&Peek(LEFT_SQUARE,2))
    {
        int temp=current;
        while(tokens[temp].type!=RIGHT_SQUARE)temp++;
        if((temp+1<tokens.size()-1)&&tokens[temp+1].type==ASSIGN)
        {
            Match(ID);Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
            Match(LEFT_SQUARE);Add_Child(node,new ASTNode(AST_LEFT_SQUARE,Previous_Token()));
            Add_Child(node,Parse_Expression());
            Match(RIGHT_SQUARE);Add_Child(node,new ASTNode(AST_RIGHT_SQUARE,Previous_Token()));
            if(Match(ASSIGN))Add_Child(node,new ASTNode(AST_ASSIGN,Previous_Token()));
            else Parse_Error("Assign character = loss.");
        }
    }

    Add_Child(node,Parse_Equality_Expression());

    return node; 
}



ASTNode* Parser::Parse_Equality_Expression()
{
    WhoAmI("Parse_Equality_Expression");

    ASTNode* node=new ASTNode(EQUALITY_EXPRESSION);

    Add_Child(node,Parse_Relational_Expression());

    while(Match(EQUAL)||Match(NOT_EQUAL))
    {
        if(Previous_Token().type==EQUAL)
            Add_Child(node,new ASTNode(AST_EQUAL,Previous_Token()));
        else if(Previous_Token().type==NOT_EQUAL)
            Add_Child(node,new ASTNode(AST_NOT_EQUAL,Previous_Token()));
         
        Add_Child(node,Parse_Relational_Expression()); 
    }

    return node;
}

ASTNode* Parser::Parse_Relational_Expression()
{
    WhoAmI("Parse_Relational_Expression");

    ASTNode* node=new ASTNode(RELATIONAL_EXPRESSION);

    Add_Child(node,Parse_PlusMinus_Expression());

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
        
        Add_Child(node,Parse_PlusMinus_Expression());
    }

    return node;
}



ASTNode* Parser::Parse_PlusMinus_Expression()
{
    WhoAmI("Parse_PlusMinus_Expression");

    ASTNode* node=new ASTNode(PLUSMINUS_EXPRESSION);

    Add_Child(node,Parse_MulDiv_Expression());
   
    while(Match(PLUS)||Match(MINUS))
    {
        if(Previous_Token().type==PLUS)
        {
            Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
        }
        else if(Previous_Token().type==MINUS)Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));

        Add_Child(node,Parse_MulDiv_Expression());
    }

    return node;
}

ASTNode* Parser::Parse_MulDiv_Expression()
{
    WhoAmI("Parse_MulDiv_Expression");

    ASTNode* node= new ASTNode(MULDIV_EXPRESSION);

    Add_Child(node,Parse_Unary_Expression());

    while(Match(STAR)||Match(SLASH))
    {
        if(Previous_Token().type==STAR)
        {
            Add_Child(node,new ASTNode(AST_STAR,Previous_Token()));
        }
        else  if(Previous_Token().type==SLASH)Add_Child(node,new ASTNode(AST_SLASH,Previous_Token()));

        Add_Child(node,Parse_Unary_Expression());
    }

    return node;
}



ASTNode* Parser::Parse_Unary_Expression()
{
    WhoAmI("Parse_Unary_Expression");

    ASTNode* node=new ASTNode(UNARY_EXPRESSION);

    Add_Child(node,Parse_Primary_Expression());
    
    return node;
}

ASTNode* Parser::Parse_Primary_Expression()
{
    WhoAmI("Parse_Primary_Expression");

    ASTNode* node=new ASTNode(PRIMARY_EXPRESSION);

    if(Match(CONSTANT_INT))Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
    else if(Match(CONSTANT_CHAR))Add_Child(node,new ASTNode(AST_CONSTANT_CHAR,Previous_Token()));
    else if(Match(CONSTANT_STRING))Add_Child(node,new ASTNode(AST_CONSTANT_STRING,Previous_Token()));
    else if(Peek(ID))
    {
        if(Peek(LEFT_PAREN,2))Add_Child(node,Parse_FunctionCall_Expression());
        else if(Peek(LEFT_SQUARE,2))Add_Child(node,Parse_Array_Expression());
        else 
        {
            Match(ID);
            Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        }
    }
    else if(Peek(AMPERSAND))Add_Child(node,Parse_Address_Expression());
    else if(Peek(STAR))Add_Child(node,Parse_Dreference_Expression());

    else Parse_Error("Primary character loss.");

    return node;
}



ASTNode* Parser::Parse_FunctionCall_Expression()
{
    WhoAmI("Parse_FunctionCall_Expression");

    ASTNode* node=new ASTNode(FUNCTIONCALL_EXPRESSION);

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            Add_Child(node,Parse_Expression());
            if(Match(RIGHT_PAREN))Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
            else Parse_Error("Right paren ) loss.");
        }
        else Parse_Error("Left paren ( loss.");
    }
    else Parse_Error("Identifier loss.");

    return node;
}



ASTNode* Parser::Parse_Address_Expression()
{
    WhoAmI("Parse_Address_Expression");

    ASTNode* node=new ASTNode(ADDRESS_EXPRESSION);

    if(Match(AMPERSAND))
    {
        Add_Child(node,new ASTNode(AST_AMPERSAND,Previous_Token()));
        if(Match(ID))
        {
            Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
            if(Match(PLUS)||Match(MINUS))
            {
                if(Previous_Token().type==PLUS)
                    Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
                else if(Previous_Token().type==MINUS)
                    Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));
                
                if(Match(CONSTANT_INT))
                    Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
                else Parse_Error("Int value loss.");
            }
        }
        else Parse_Error("Identifier loss.");
    }
    else Parse_Error("Ampersand & loss.");

    return node;
}

ASTNode* Parser::Parse_Dreference_Expression()
{
    WhoAmI("Parse_Dreference_Expression");

    ASTNode* node=new ASTNode(DREFERENCE_EXPRESSION);

    if(Match(STAR))
    {
        Add_Child(node,new ASTNode(AST_STAR,Previous_Token()));
        if(Match(ID))
        {
            Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        }
        else if(Match(LEFT_PAREN))
        {
            Add_Child(node,new ASTNode(AST_LEFT_PAREN,Previous_Token()));
            if(Match(ID))
            {
                Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
                if(Match(PLUS)||Match(MINUS))
                {
                    if(Previous_Token().type==PLUS)
                        Add_Child(node,new ASTNode(AST_PLUS,Previous_Token()));
                    else if(Previous_Token().type==MINUS)
                        Add_Child(node,new ASTNode(AST_MINUS,Previous_Token()));
                
                    if(Match(CONSTANT_INT))
                        Add_Child(node,new ASTNode(AST_CONSTANT_INT,Previous_Token()));
                    else Parse_Error("Int value loss.");

                    if(Match(RIGHT_PAREN))Add_Child(node,new ASTNode(AST_RIGHT_PAREN,Previous_Token()));
                    else Parse_Error("Right paren ) loss.");
                }
            }
            else Parse_Error("Identifier loss.");
        }
        else Parse_Error("Identifier loss.");
    }
    else Parse_Error("Star * loss.");

    return node;
}

ASTNode* Parser::Parse_Array_Expression()
{
    WhoAmI("Parse_Array_Expression");

    ASTNode* node=new ASTNode(ARRAY_EXPRESSION);

    if(Match(ID))
    {
        Add_Child(node,new ASTNode(AST_ID,Previous_Token()));
        if(Match(LEFT_SQUARE))
        {
            Add_Child(node,new ASTNode(AST_LEFT_SQUARE,Previous_Token()));
            Add_Child(node,Parse_Expression());
            if(Match(RIGHT_SQUARE))Add_Child(node,new ASTNode(AST_RIGHT_SQUARE,Previous_Token()));
            else Parse_Error("Right square ] loss.");
        }
        else Parse_Error("Left square [ loss.");
    }
    else Parse_Error("Identifier loss.");

    return node;
}
 



//Tools
bool Parser::Match(TokenType expected)
{
    if(!Is_AtEnd()&&expected==tokens[current].type)
    {
        current++;
        return true;
    }
    return false;
}

void Parser::Match_Semicolon(ASTNode* root)
{
    if(Match(SEMICOLON))Add_Child(root,new ASTNode(AST_SEMICOLON,Previous_Token()));        
    else Parse_Error("Semicolon ; loss.");
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

bool Parser::Peek_Type()
{
    return Peek(VOID)||Peek(CHAR)||Peek(INT)||Peek(LONG);
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



bool Parser::Is_AtEnd()
{
    return tokens[current].type==CODE_EOF;
}



void Parser::WhoAmI(string name)
{
    if(DEBUG)cout<<name<<endl;
}

void Parser::Parse_Error(string error_message)
{
    cout<< "Parse Error: Line "<<tokens[current].line<<": "<<error_message<<endl;
    exit(2);
}
