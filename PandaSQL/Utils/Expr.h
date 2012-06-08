#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

#include "Utils/Types.h"

namespace PandaSQL
{
	
enum ExprType
{
	kExprNumber = 0,
	kExprText = 1,
	kExprColumnDef = 2
};

struct Expr
{
	ExprType type;

	//type == kExprNumber
	int32_t number;

	//type == kExprText
	std::string text;

	//type == kExprColumnDef
	ColumnDef columnDef;
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H
