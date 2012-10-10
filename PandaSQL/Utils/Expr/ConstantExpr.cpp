#include "stdafx.h"

#include "ConstantExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

ConstantExpr::ConstantExpr()
:
Expr(kExprConstant)
{
}

ConstantExpr::~ConstantExpr()
{
}

int32_t ConstantExpr::GetNumber() const
{
	return mConstantValue.GetAsNumber();
}

void ConstantExpr::SetNumber(int32_t inNumberValue)
{
	mConstantValue.SetAsNumber(inNumberValue);
}
	
std::string ConstantExpr::GetText() const
{
	return mConstantValue.GetAsString();
}

void ConstantExpr::SetText(const std::string inTextValue)
{
	mConstantValue.SetAsString(inTextValue);
}

bool ConstantExpr::IsTrue(const ExprContext &inExprContext) const
{
	return true;
}

Status ConstantExpr::GetValue(const ExprContext &inExprContext, Value *io_value) const
{
	Status result;

	*io_value = mConstantValue;

	return result;
}

}	// PandaSQL
