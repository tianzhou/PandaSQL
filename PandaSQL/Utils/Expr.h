#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

#include "Utils/Types.h"

namespace PandaSQL
{
	
enum ExprType
{
	kExprUnknown = 0,
	kExprNumber = 1,
	kExprText = 2,
	kExprColumnDef = 3
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
