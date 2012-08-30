#include "stdafx.h"

#include "BinaryExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

BinaryExpr::BinaryExpr()
:
mOpType(kBinaryUnknown)
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

}	// PandaSQL