#include "stdafx.h"

#include "BooleanExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

BooleanExpr::BooleanExpr()
:
mType(kBooleanUnknown)
,mNegate(false)
{
}

BooleanExpr::~BooleanExpr()
{
}

void BooleanExpr::SetNegate(bool isNegate)
{
	mNegate = isNegate;
}

void BooleanExpr::SetType(BooleanType inType)
{
	mType = inType;
}

const BooleanExpr::BooleanList& BooleanExpr::GetBooleanList() const
{
	return mBooleanList;
}

void BooleanExpr::AddExpr(const Expr *inExpr)
{
	mBooleanList.push_back(inExpr);
}

}	// PandaSQL