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
		kBooleanSingle = 1,
		kBooleanAndList = 2,
		kBooleanOrList = 3
	};

	typedef uint8_t BooleanType;

	typedef std::vector<Expr *> BooleanList;

	BooleanExpr();
	~BooleanExpr();

	void SetNegate(bool isNegate);
	void SwitchNegate();

	void SetBooleanType(BooleanType inType);

	const BooleanList& GetBooleanList() const;
	void AddExpr(Expr *inExpr);
	
	void TransformToCNF();

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual void MutableWalk(MutableExprWalker *io_walker);
	virtual Expr* Clone() const;
	virtual void  Print(uint32_t level) const;

private:

	void Flatten_Private();
	
	BooleanType mBooleanType;
	bool		mNegate;
	BooleanList mBooleanList;
};

}	// PandaSQL


#endif //PANDASQL_BOOLEAN_EXPR_H
