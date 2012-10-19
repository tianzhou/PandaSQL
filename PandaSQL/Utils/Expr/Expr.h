#ifndef PANDASQL_EXPR_H
#define PANDASQL_EXPR_H

#include "Access/Tuple.h"
#include "Catalog/Column.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class Bitmask;
class Expr;
class ExprContext;
class ExprWalker;
class Value;

enum ExprType
{
	kExprUnknown = 0,
	kExprNumber = 1,
	kExprText = 2,
	kExprColumnDef = 3
};

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

	~Expr();

	ExprType GetType() const { return mExprType; }

	void Eval(TupleDescElement inDescElement, TupleDataElement *io_data) const;

	static void EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, TupleData *io_tupleData);

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	virtual Status GetValue(const ExprContext &inExprContext, Value *io_value) const;

	//We are only interested in those expressions referencing any table names in <<tableNameList>>
	//This is useful for predicate pushdown
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual Expr* Clone() const;

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
