#include "stdafx.h"

#include "BinaryExpr.h"

#include "Utils/Debug.h"
#include "Utils/Value.h"

namespace PandaSQL
{

BinaryExpr::BinaryExpr()
:
Expr(kExprBinary)
,mOpType(kBinaryUnknown)
,mpLeftOperand(NULL)
,mpRightOperand(NULL)
{
}

BinaryExpr::~BinaryExpr()
{
}

BinaryExpr::BinaryOpType BinaryExpr::GetOpType() const
{
	return mOpType;
}

void BinaryExpr::SetOpType(BinaryExpr::BinaryOpType inOpType)
{
	mOpType = inOpType;
}

const Expr* BinaryExpr::GetLeftOperand() const
{
	return mpLeftOperand;
}

void BinaryExpr::SetLeftOperand(const Expr *inLeftOperand)
{
	mpLeftOperand = inLeftOperand;
}

const Expr* BinaryExpr::GetRightOperand() const
{
	return mpRightOperand;
}

void BinaryExpr::SetRightOperand(const Expr *inRightOperand)
{
	mpRightOperand = inRightOperand;
}

bool BinaryExpr::IsTrue(ExprContext *io_exprContext) const
{
	bool result = true;

	PDASSERT(mpLeftOperand && mpRightOperand);

	if (mpLeftOperand && mpRightOperand)
	{
		Value leftValue, rightValue;
		
		mpLeftOperand->GetValue(io_exprContext, &leftValue);
		mpRightOperand->GetValue(io_exprContext, &rightValue);

		// If the current context doesn't include the value for the operand,
		// We just assume it is true, which means it's too early to evaluate.
		if (leftValue.GetType() == kUnknownType
			|| rightValue.GetType() == kUnknownType)
		{
			result = true;
		}
		else
		{
			switch (mOpType)
			{
			case kBinaryEqual:
				{
					result = (leftValue == rightValue);
					break;
				}
			default:
				PDASSERT(0);
				break;
			}
		}
	}

	return result;
}

}	// PandaSQL