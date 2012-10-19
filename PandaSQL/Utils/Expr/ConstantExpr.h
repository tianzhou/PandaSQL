#ifndef PANDASQL_CONSTANT_EXPR_H
#define PANDASQL_CONSTANT_EXPR_H

#include "Expr.h"

#include "Utils/Value.h"

namespace PandaSQL
{

class ConstantExpr : public Expr
{
public:

	enum 
	{
		kConstantUnknown = 0,
		kConstantInt = 1,
		kConstantText = 2,
	};

	typedef uint8_t ConstantType;

	ConstantExpr();
	~ConstantExpr();

	int32_t GetNumber() const;
	void SetNumber(int32_t inNumberValue);
	
	std::string GetText() const;
	void SetText(const std::string inTextValue);

	virtual bool IsTrue(const ExprContext &inExprContext) const;
	virtual Status GetValue(const ExprContext &inExprContext, Value *io_value) const;
	
	virtual Expr* CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const;

	virtual Expr* Clone() const;

private:

	ConstantType mConstantType;
	
	Value mConstantValue;
};

}	// PandaSQL


#endif //PANDASQL_CONSTANT_EXPR_H
