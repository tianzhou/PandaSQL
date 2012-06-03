#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

namespace PandaSQL
{
	
enum ExprType
{
	kExprNumber = 0,
	kExprText = 1,
	kExprColumnRef = 2
};

struct Expr
{
	ExprType type;
	std::string text;
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H
