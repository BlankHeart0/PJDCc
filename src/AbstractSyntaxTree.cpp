#include "AbstractSyntaxTree.h"

void AST::AST_Print(ASTNode* root,int depth)
{   
    for(int i=0;i<depth;i++)cout<<'\t';
    
    cout<<ASTNodeType_text[root->type];

    if(root->type==AST_CONSTANT_INT)cout<<" "<<root->literal_int<<" --line:"<<root->line;
    else if(root->type==AST_CONSTANT_CHAR)cout<<" "<<root->literal_char<<" --line:"<<root->line;
    else if(root->type==AST_CONSTANT_STRING)cout<<" "<<root->literal_string<<" --line:"<<root->line;
    else if(root->type==AST_ID)cout<<" "<<root->lexeme<<" "<<root->line;

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



void AST::AST_Clean(ASTNode* root)
{
    for(ASTNode* node:root->Children)
    {
        delete node;
    }
    
    delete root;
}

void AST::AST_Clean()
{

    AST_Clean(Translation);

}