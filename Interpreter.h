#include "AbstractSyntaxTree.h"

//Seems like Parser
class Interpreter
{
public:
    AST ast;

    bool DEBUG;
    
    Interpreter():DEBUG(true){}

    void Interpret();

    void Interpret_Translation_Unit(ASTNode* root);

    void Interpret_Expression(ASTNode* root);
    void Interpret_PlusMinus_Expression(ASTNode* root);
    void Interpret_MulDiv_Expression(ASTNode* root);
    void Interpret_Unary_Expression(ASTNode* root);
    void Interpret_Primary_Expression(ASTNode* root);

    ASTNode* FirstChild(ASTNode* root);
    void RootValue_Print(ASTNode* root);
};