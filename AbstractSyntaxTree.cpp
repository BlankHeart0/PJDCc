#include "AbstractSyntaxTree.h"

void AST::AST_Print(ASTNode* root,int depth)
{   
    for(int i=0;i<depth;i++)cout<<'\t';
    
    cout<<ASTNodeType_text[root->type];

    if(root->type==AST_CONSTANT_INT)cout<<" "<<root->literal<<" "<<root->line;
    else if(root->type==AST_ID)cout<<" "<<root->lexeme<<" "<<root->line;

    cout<<endl;

    for(ASTNode* ast_p:root->Children)
    { 
        AST_Print(ast_p,depth+1);
    }
    
}

void AST::AST_Print()
{
    cout<<endl<<"----- The AST -----"<<endl;

    AST_Print(Translation,0);

    cout<<endl<<"----- Print Done -----"<<endl;
}