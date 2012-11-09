#ifndef PANDASQL_BOOLEAN_EXPR_H
#define PANDASQL_BOOLEAN_EXPR_H

#include "Expr/Expr.h"

namespace PandaSQL
{

class BooleanExpr : public Expr
{

public:

	enum
	{
		kBooleanUnknown = 0,
		kBooleanNormal = 1,
		kBooleanAndList = 2,
		kBooleanOrList = 3
	};

	typedef uint8_t BooleanType;

	typedef std::vector<Expr *> BooleanList;

	BooleanExpr();
	~BooleanExpr();

	void SetNegate(bool isNegate);

	void SetType(BooleanType inType);

	const BooleanList& GetBooleanList() const;
	void AddExpr(Expr *inExpr);
	
	void TransformToCNF();
	void Print(uint32_t level) const;

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual void MutableWalk(MutableExprWalker *io_walker);
	virtual Expr* Clone() const;

protected:
	
	BooleanType mType;
	bool		mNegate;
	BooleanList mBooleanList;
};

}	// PandaSQL


#endif //PANDASQL_BOOLEAN_EXPR_H
