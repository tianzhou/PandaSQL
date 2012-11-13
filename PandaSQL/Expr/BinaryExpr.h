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
		kBinaryNotEqual = 2,
		kBinaryGreater = 3,
		kBinaryGreaterEqual = 4,
		kBinaryLess = 5,
		kBinaryLessEqual = 6
	};

	typedef uint8_t BinaryOpType;

	BinaryExpr();
	~BinaryExpr();

	BinaryOpType GetOpType() const;
	void SetOpType(BinaryOpType inOpType);

	Expr* GetLeftOperand() const;
	void SetLeftOperand(Expr *inLeftOperand);
	
	Expr* GetRightOperand() const;
	void SetRightOperand(Expr *inRightOperand);

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual void Walk(ExprWalker *io_walker) const;
	virtual void MutableWalk(MutableExprWalker *io_walker);
	virtual Expr* Clone() const;
	virtual void  Print(uint32_t level) const;

private:

	BinaryOpType mOpType;

	Expr *mpLeftOperand;
	Expr *mpRightOperand;
};

}	// PandaSQL


#endif //PANDASQL_BINARY_EXPR_H
