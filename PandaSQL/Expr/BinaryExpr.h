#ifndef PANDASQL_BINARY_EXPR_H
#define PANDASQL_BINARY_EXPR_H

#include "Expr/Expr.h"

namespace PandaSQL
{

class BinaryExpr : public Expr
{
public:

	enum 
	{
		kBinaryUnknown = 0,
		kBinaryEqual = 1,
	};

	typedef uint8_t BinaryOpType;

	BinaryExpr();
	~BinaryExpr();

	BinaryOpType GetOpType() const;
	void SetOpType(BinaryOpType inOpType);

	const Expr* GetLeftOperand() const;
	void SetLeftOperand(const Expr *inLeftOperand);
	
	const Expr* GetRightOperand() const;
	void SetRightOperand(const Expr *inRightOperand);

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual Expr* Clone() const;

private:

	BinaryOpType mOpType;
	
	const Expr *mpLeftOperand;
	const Expr *mpRightOperand;
};

}	// PandaSQL


#endif //PANDASQL_BINARY_EXPR_H
