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
class MutableExprWalker;
class Value;

enum ExprType
{
	kExprUnknown = 0,
	kExprNumber = 1,
	kExprText = 2,
	kExprColumnDef = 3
};

typedef std::vector<Expr*> ExprList;

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

	static Status EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, ValueList *io_tupleValueList);

	Expr();

	Expr(ExprType inExprType);

	virtual ~Expr();

	ExprType GetType() const { return mExprType; }

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	virtual Status GetValue(const ExprContext &inExprContext, Value *io_value) const;

	//We are only interested in those expressions referencing any table names in <<tableNameList>>
	//This is useful for predicate pushdown
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual void MutableWalk(MutableExprWalker *io_walker);

	virtual Expr* Clone() const;

	ExprType mExprType;

protected:

	
};

}	// PandaSQL


#endif //PANDASQL_EXPR_H
