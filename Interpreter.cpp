#include "Interpreter.h"
#include "AbstractSyntaxTree.h"

void Interpreter::Interpret()
{
    cout<<endl<<"--- Interpret Begin ---"<<endl;
    Interpret_Translation_Unit(A.Translation);
    cout<<"Expression Calculate Result : "<<A.Translation->value<<endl;
    cout<<"--- Interpret Done ---"<<endl<<endl;
}

void Interpreter::Interpret_Translation_Unit(ASTNode* root)
{
    Interpret_Expression(FirstChild(root));
    root->value=FirstChild(root)->value;

    RootValue_Print(root);
}

void Interpreter::Interpret_Expression(ASTNode* root)
{   
    Interpret_PlusMinus_Expression(FirstChild(root));
    root->value=FirstChild(root)->value;

    RootValue_Print(root);
}

void Interpreter::Interpret_PlusMinus_Expression(ASTNode* root)
{
    Interpret_MulDiv_Expression(FirstChild(root));
    root->value=FirstChild(root)->value;

    for(int i=1;i<root->Children.size();i+=2)
    {
        Interpret_MulDiv_Expression(root->Children[i+1]);
        if(root->Children[i]->type==AST_PLUS)root->value+=root->Children[i+1]->value;
        else if(root->Children[i]->type==AST_MINUS)root->value-=root->Children[i+1]->value;
    }

    RootValue_Print(root);
}

void Interpreter::Interpret_MulDiv_Expression(ASTNode* root)
{
    Interpret_Unary_Expression(FirstChild(root));
    root->value=FirstChild(root)->value;

    for(int i=1;i<root->Children.size();i+=2)
    {
        Interpret_Unary_Expression(root->Children[i+1]);
        if(root->Children[i]->type==AST_STAR)root->value*=root->Children[i+1]->value;
        else if(root->Children[i]->type==AST_SLASH)root->value/=root->Children[i+1]->value;
    }

    RootValue_Print(root);
}

void Interpreter::Interpret_Unary_Expression(ASTNode* root)
{
    if(FirstChild(root)->type==PRIMARY_EXPRESSION)
    {
        Interpret_Primary_Expression(FirstChild(root));
        root->value=FirstChild(root)->value;
    }
    else 
    { 
        if(FirstChild(root)->type==AST_PLUS)
        {
            Interpret_Unary_Expression(root->Children[1]);
            root->value=root->Children[1]->value;
        }
        else if(FirstChild(root)->type==AST_MINUS)
        {
            Interpret_Unary_Expression(root->Children[1]);
            root->value=-(root->Children[1]->value);
        }
    }

    RootValue_Print(root);
}

void Interpreter::Interpret_Primary_Expression(ASTNode* root)
{
    root->value=FirstChild(root)->value;

    RootValue_Print(root);
}



ASTNode* Interpreter::FirstChild(ASTNode* root)
{
    if(root->Children.size()>=1)return root->Children[0];
    return nullptr;
}

void Interpreter::RootValue_Print(ASTNode* root)
{
    if(DEBUG)cout<<root->value<<endl;
}