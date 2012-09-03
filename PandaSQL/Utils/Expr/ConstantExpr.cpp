#include "stdafx.h"

#include "ConstantExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

ConstantExpr::ConstantExpr()
:
Expr(kExprConstant)
,mConstantType(kConstantUnknown)
{
}

ConstantExpr::~ConstantExpr()
{
}

ConstantExpr::ConstantType ConstantExpr::GetType() const
{
	return mConstantType;
}

void ConstantExpr::SetType(ConstantExpr::ConstantType inType)
{
	mConstantType = inType;
}

int32_t ConstantExpr::GetInt() const
{
	PDASSERT(mConstantType == kConstantInt);
	return mIntValue;
}

void ConstantExpr::SetInt(int32_t inIntValue)
{
	this->SetType(kConstantInt);
	mIntValue = inIntValue;
}
	
std::string ConstantExpr::GetText() const
{
	PDASSERT(mConstantType == kConstantText);
	return mTextValue;
}

void ConstantExpr::SetText(const std::string inTextValue)
{
	this->SetType(kConstantText);
	mTextValue = inTextValue;
}

}	// PandaSQL
