#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

#include "Access/Tuple.h"
#include "Catalog/Column.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class ExprContext;
class Value;
	
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
		kExprUnknown = 0,
		kExprColumnRef = 1,
		kExprConstant = 2,
		kExprBoolean = 3,
		kExprBinary = 4
	};

	typedef uint8_t ExprType;

	Expr();

	Expr(ExprType inExprType);

	void Eval(TupleDescElement inDescElement, TupleDataElement *io_data) const;

	static void EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, TupleData *io_tupleData);

	virtual bool IsTrue(ExprContext *io_exprContext) const;
	virtual Status GetValue(ExprContext *io_exprContext, Value *io_value) const;

	virtual void PopulateDependentColumns(TableAndColumnSetMap *io_tableAndColumnSetMap) const;

	ExprType mExprType;

	//type == kExprNumber
	int32_t mNumberValue;

	//type == kExprText
	std::string mTextValue;

	//type == kExprColumnDef
	ColumnDef mColumnDef;

protected:

	
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H
