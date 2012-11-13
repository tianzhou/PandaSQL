#include "stdafx.h"

#include "Expr/BooleanExpr.h"

#include "Expr/ExprWalker.h"

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
	BooleanList::iterator iter = mBooleanList.begin();

	for (; iter != mBooleanList.end(); iter++)
	{
		delete *iter;
	}

	mBooleanList.clear();
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

void BooleanExpr::AddExpr(Expr *inExpr)
{
	mBooleanList.push_back(inExpr);
}

void BooleanExpr::TransformToCNF()
{
	//if (mLogicGateType == kLogicAnd
	//	|| mLogicGateType == kLogicOr)
	//{
	//	std::vector<Predicate>::iterator iter = mPredicateList.begin();

	//	while (iter != mPredicateList.end())
	//	{
	//		iter->TransformToCNF();
	//		iter++;
	//	}

	//	if (mLogicGateType == kLogicOr)
	//	{
	//		PDASSERT(mPredicateList.size() >= 2);

	//		std::vector<Predicate>::iterator leftIter = mPredicateList.begin();
	//		std::vector<Predicate>::iterator rightIter = leftIter+1;
	//		std::vector<Predicate> newPredicateList;

	//		while (rightIter != mPredicateList.end())
	//		{
	//			//.......

	//			PDASSERT(leftIter->mLogicGateType == kLogicStandalone
	//				|| leftIter->mLogicGateType == kLogicAnd);

	//			PDASSERT(rightIter->mLogicGateType == kLogicStandalone
	//				|| rightIter->mLogicGateType == kLogicAnd);

	//			std::vector<Predicate> leftPredicateList;
	//			std::vector<Predicate> rightPredicateList;

	//			if (leftIter->mLogicGateType == kLogicStandalone)
	//			{
	//				leftPredicateList.push_back(*leftIter);
	//			}
	//			else
	//			{
	//				leftPredicateList = leftIter->mPredicateList;
	//			}

	//			if (rightIter->mLogicGateType == kLogicStandalone)
	//			{
	//				rightPredicateList.push_back(*rightIter);
	//			}
	//			else
	//			{
	//				rightPredicateList = rightIter->mPredicateList;
	//			}


	//			std::vector<Predicate>::iterator subLeftIter = leftPredicateList.begin();
	//			
	//			for (;subLeftIter != leftPredicateList.end();subLeftIter++)
	//			{
	//				std::vector<Predicate>::iterator subRightIter = rightPredicateList.begin();
	//				std::vector<Predicate> onePredicateList;
	//				
	//				//Push the left item
	//				onePredicateList.push_back(*subLeftIter);

	//				for (;subRightIter != rightPredicateList.end();subRightIter++)
	//				{
	//					//Push the right item
	//					onePredicateList.push_back(*subRightIter);
	//					
	//					Predicate onePredicate;
	//					onePredicate.SetOrPredicateWithSubpredicates(onePredicateList);
	//					
	//					newPredicateList.push_back(onePredicate);
	//					
	//					//Pop the right item
	//					onePredicateList.pop_back();
	//				}		
	//			}

	//			leftIter = rightIter;
	//			rightIter++;
	//		}
	//		
	//		this->Reset();

	//		this->SetAndPredicateWithSubpredicates(newPredicateList);
	//	}
	//}
}

void BooleanExpr::Print(uint32_t level) const
{
	if (mType != kBooleanNormal)
	{
		this->PrintIndention(level);
		level++;
	}

	switch (mType)
	{
	case kBooleanNormal:
		{
			break;
		}
	case kBooleanAndList:
		{
			std::cout << "AND" << std::endl;
			break;
		}
	case kBooleanOrList:
		{
			std::cout << "OR" << std::endl;
			break;
		}
	case kBooleanUnknown:
	default:
		{
			PDASSERT(0);
			break;
		}
	}

	BooleanList::const_iterator iter = mBooleanList.begin();
	for (; iter != mBooleanList.end(); iter++)
	{
		(*iter)->Print(level);
	}

	//if (mLogicGateType != kLogicUnknown)
	//{
	//	std::string identation;

	//	for (uint32_t i = 0; i < level; i++)
	//	{
	//		identation += "  ";
	//	}

	//	if (mLogicGateType == kLogicAnd
	//		|| mLogicGateType == kLogicOr)
	//	{
	//		if (mLogicGateType == kLogicAnd)
	//		{
	//			std::cout << identation << "And" << std::endl;
	//		}
	//		else
	//		{
	//			std::cout << identation << "Or" << std::endl;
	//		}

	//		identation += "  ";

	//		std::vector<Predicate>::const_iterator iter = mPredicateList.begin();

	//		for (; iter != mPredicateList.end(); iter++)
	//		{
	//			iter->Print(level + 1);
	//		}
	//	}
	//	else if (mLogicGateType == kLogicStandalone)
	//	{
	//		mPredicateItem.Print(level + 1);
	//	}
	//}	
}

bool BooleanExpr::IsTrue(const ExprContext &inExprContext) const
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
				if (!(*iter)->IsTrue(inExprContext))
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
				if ((*iter)->IsTrue(inExprContext))
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

			result = mBooleanList[0]->IsTrue(inExprContext);
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

Expr* BooleanExpr::CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const
{
	Expr *result = NULL;

	//We should at least one bit set, which means we are trying
	//to create requried predicate expr for the leaf node
	PDASSERT(inTableNameList.size() > 0);

	switch (mType)
	{
	case kBooleanAndList:
		{		
			PDASSERT(!mBooleanList.empty());

			BooleanExpr *multipleExpr = NULL;
			Expr *firstExpr = NULL;

			BooleanList::const_iterator booleanListIter = mBooleanList.begin();
			for (; booleanListIter != mBooleanList.end(); booleanListIter++)
			{
				Expr *newExpr = (*booleanListIter)->CreateSubExprForPushdown(inTableNameList);

				if (newExpr)
				{
					if (firstExpr)
					{
						if (!multipleExpr)
						{
							multipleExpr = new BooleanExpr();
							multipleExpr->SetType(kBooleanAndList);

						}

						multipleExpr->AddExpr(firstExpr);
						multipleExpr->AddExpr(newExpr);
					}
					else
					{
						firstExpr = newExpr;
					}
				}
			}

			if (multipleExpr)
			{
				result = multipleExpr;
			}
			else
			{
				result = firstExpr;
			}

			break;
		}
	case kBooleanOrList:
		{
			PDASSERT(!mBooleanList.empty());

			BooleanExpr *multipleExpr = NULL;
			Expr *firstExpr = NULL;

			BooleanList::const_iterator booleanListIter = mBooleanList.begin();
			for (; booleanListIter != mBooleanList.end(); booleanListIter++)
			{
				Expr *newExpr = (*booleanListIter)->CreateSubExprForPushdown(inTableNameList);

				if (newExpr)
				{
					if (firstExpr)
					{
						if (!multipleExpr)
						{
							multipleExpr = new BooleanExpr();
							multipleExpr->SetType(kBooleanOrList);

						}

						multipleExpr->AddExpr(firstExpr);
						multipleExpr->AddExpr(newExpr);
					}
					else
					{
						firstExpr = newExpr;
					}
				}
				else
				{
					//As long as there is one sub expression doesn't have enough context,
					//we will exclude the whole expression
					if (multipleExpr)
					{
						delete multipleExpr;
						multipleExpr = NULL;
					}
					else if (firstExpr)
					{
						delete firstExpr;
						firstExpr = NULL;
					}
				}
			}

			if (multipleExpr)
			{
				result = multipleExpr;
			}
			else
			{
				result = firstExpr;
			}


			break;
		}
	case kBooleanNormal:
		{
			PDASSERT(mBooleanList.size() == 1);

			result = mBooleanList[0]->CreateSubExprForPushdown(inTableNameList);

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

void BooleanExpr::Walk(ExprWalker *io_walker) const
{
	BooleanList::const_iterator iter = mBooleanList.begin();
	for (; iter != mBooleanList.end(); iter++)
	{
		(*iter)->Walk(io_walker);
	}
}

void BooleanExpr::MutableWalk(MutableExprWalker *io_walker)
{
	BooleanList::iterator iter = mBooleanList.begin();
	for (; iter != mBooleanList.end(); iter++)
	{
		(*iter)->MutableWalk(io_walker);
	}
}

Expr* BooleanExpr::Clone() const
{
	BooleanExpr *result = new BooleanExpr();

	result->mType = mType;
	result->mNegate = mNegate;
	
	uint32_t i;
	for (i = 0; i < mBooleanList.size(); i++)
	{
		Expr *theExpr = mBooleanList[i]->Clone();
		result->mBooleanList.push_back(theExpr);
	}

	return result;
}

}	// PandaSQL