#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

#include "Access/Tuple.h"
#include "Catalog/Column.h"

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

class Expr;

typedef std::vector<Expr> ExprList;

class Expr
{

public:

	enum
	{
		kExprColumnRef,
	};

	typedef uint8_t ExprType;

	void Eval(TupleDescElement inDescElement, TupleDataElement *io_data) const;

	static void EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, TupleData *io_tupleData);

	ExprType mType;

	//type == kExprNumber
	int32_t mNumberValue;

	//type == kExprText
	std::string mTextValue;

	//type == kExprColumnDef
	ColumnDef mColumnDef;
};

class ColumnExpr : public Expr
{
public:

};

}	// PandaSQL


#endif //PANDASQL_EXPR_H
