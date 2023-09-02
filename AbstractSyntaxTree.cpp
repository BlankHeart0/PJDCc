#include "AbstractSyntaxTree.h"

#include <iostream>
using namespace std;

void AST::AST_Print(ASTNode* root,int depth)
{   
    for(int i=0;i<depth;i++)cout<<'\t';
    cout<<ASTNodeType_text[root->type];
    if(root->type==AST_CONSTANT_INT)cout<<root->value;
    cout<<endl;

    for(ASTNode* ast_p:root->Children)
    { 
        AST_Print(ast_p,depth+1);
    }
    
}

void AST::AST_Print()
{
    AST_Print(Translation,0);
}