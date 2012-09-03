#include "stdafx.h"

#include "BooleanExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

BooleanExpr::BooleanExpr()
:
Expr(kExprBoolean)
,mType(kBooleanUnknown)
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

bool BooleanExpr::IsTrue(ExprContext *io_exprContext) const
{
	bool result = true;

	switch (mType)
	{
	case kBooleanAndList:
		{		
			PDASSERT(!mBooleanList.empty());

			BooleanList::const_iterator iter = mBooleanList.begin();
			for (; iter != mBooleanList.end(); iter++)
			{
				if (!(*iter)->IsTrue(io_exprContext))
				{
					break;
				}
			}

			if (iter == mBooleanList.end())
			{
				result = true;
			}
			else
			{
				result = false;
			}

			break;
		}
	case kBooleanOrList:
		{
			PDASSERT(!mBooleanList.empty());

			BooleanList::const_iterator iter = mBooleanList.begin();
			for (; iter != mBooleanList.end(); iter++)
			{
				if ((*iter)->IsTrue(io_exprContext))
				{
					break;
				}
			}

			if (iter == mBooleanList.end())
			{
				result = false;
			}
			else
			{
				result = true;
			}

			break;
		}
	case kBooleanNormal:
		{
			PDASSERT(mBooleanList.size() == 1);

			result = mBooleanList[0]->IsTrue(io_exprContext);
			break;
		}
	case kBooleanUnknown:
		{
			PDASSERT(0);
			break;
		}
	}

	return result;
}

}	// PandaSQL