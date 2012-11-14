#include "stdafx.h"

#include "Expr/BooleanExpr.h"

#include "Expr/ExprWalker.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

static void CleanBooleanList_LocalStatic(BooleanExpr::BooleanList *io_booleanList)
{
	BooleanExpr::BooleanList::iterator iter = io_booleanList->begin();

	for (; iter != io_booleanList->end(); iter++)
	{
		delete *iter;
	}

	io_booleanList->clear();
}

BooleanExpr::BooleanExpr()
:
Expr(kExprBoolean)
,mBooleanType(kBooleanUnknown)
,mNegate(false)
{
}

BooleanExpr::~BooleanExpr()
{
	//CleanBooleanList_LocalStatic(&mBooleanList);
}

void BooleanExpr::SetNegate(bool isNegate)
{
	mNegate = isNegate;
}

void BooleanExpr::SwitchNegate()
{
	mNegate = !mNegate;
}

void BooleanExpr::SetBooleanType(BooleanType inType)
{
	mBooleanType = inType;
}

const BooleanExpr::BooleanList& BooleanExpr::GetBooleanList() const
{
	return mBooleanList;
}

void BooleanExpr::AddExpr(Expr *inExpr)
{
	mBooleanList.push_back(inExpr);
}

void BooleanExpr::Flatten_Private()
{
	for (BooleanList::iterator iter = mBooleanList.begin(); iter != mBooleanList.end(); iter++)
	{
		if ((*iter)->GetExprType() == kExprBoolean)
		{
			dynamic_cast<BooleanExpr *>(*iter)->Flatten_Private();
		}
	}

	if (mBooleanType == kBooleanSingle
		&& mBooleanList[0]->GetExprType() == kExprBoolean)
	{
		//Parser should make sure single boolean expr
		//shouldn't contain boolean list
		PDASSERT(0);
	}

	//"(A AND B) AND C" -> "A AND B AND C"
	//"(A OR B) OR C" -> "A OR B OR C"
	if (mBooleanType == kBooleanAndList
		|| mBooleanType == kBooleanOrList)
	{
		BooleanList::iterator iter = mBooleanList.begin();
		size_t originalSize = mBooleanList.size();
		BooleanList hoistBooleanList;

		while (iter != mBooleanList.end())
		{
			if ((*iter)->GetExprType() == kExprBoolean)
			{
				BooleanExpr *subBooleanExp = dynamic_cast<BooleanExpr *>(*iter);
			
				if (subBooleanExp->mBooleanType == mBooleanType)
				{
					const BooleanList &subBooleanList = subBooleanExp->GetBooleanList();
					
					//Prepare the booleanList under this subExpr to hoist 
					for (BooleanList::const_iterator subIter = subBooleanList.begin(); subIter != subBooleanList.end(); subIter++)
					{
						Expr *newExpr = (*subIter)->Clone();
						hoistBooleanList.push_back(newExpr);
					}

					//We no longer need this subExpr
					delete subBooleanExp;
					iter = mBooleanList.erase(iter);
				}
				else
				{
					iter++;
				}
			}
			else
			{
				iter++;
			}
		}

		mBooleanList.insert(mBooleanList.end(), hoistBooleanList.begin(), hoistBooleanList.end());

		//After flatten, we should make sure AND/OR boolean lists
		//are intertwined. e.g.(A OR B) AND (C OR D)
		for (BooleanList::const_iterator iter = mBooleanList.begin(); iter != mBooleanList.end(); iter++)
		{
			if ((*iter)->GetExprType() == kExprBoolean)
			{
				PDASSERT(dynamic_cast<BooleanExpr *>(*iter)->mBooleanType != mBooleanType);
			}
		}
	}
}

void BooleanExpr::TransformToCNF()
{
	//Flatten the expr to prepare for CNF
	//The result of this is mBooleanList should be
	//AND/OR intertwined. e.g.(A OR B) AND (C OR D)
	this->Flatten_Private();

	//We try to convert it from "And" list containing "Or" lists
	//To "Or" list containing "And" lists which is useful for index matching
	//e.g. (A OR B) AND (C OR D) -> (A AND C) OR (A AND D) OR (B AND C) OR (B AND D)
	//NOTE: We only do this for the current level.
	//Otherwise, we might generate very long list which is not very useful.
	if (mBooleanType == kBooleanAndList)
	{
		PDASSERT(mBooleanList.size() >= 2);

		//Each BooleanList in the vector would be an "And" type list
		//The final mBooleanList for this BooleanExpr is an "Or" type list
		//containing all these "And" lists
		std::vector<BooleanList> currentBooleanListList;
		
		//For the first run(i.e. First element in mBooleanList),
		//we will init the currentBooleanListList.
		//After that, we will update currentBooleanListList
		//with the next element in mBooleanList
		bool needInitCurrentBooleanListList = true;
		BooleanList::iterator booleanListIter = mBooleanList.begin();
		
		while (booleanListIter != mBooleanList.end())
		{
			std::vector<BooleanList> newBooleanListList;

			std::vector<BooleanList>::const_iterator booleanListListIter = currentBooleanListList.begin();
			while (needInitCurrentBooleanListList
				|| booleanListListIter != currentBooleanListList.end())
			{
				if ((*booleanListIter)->GetExprType() == kExprBoolean)
				{
					BooleanExpr *subBooleanExp = dynamic_cast<BooleanExpr *>(*booleanListIter);

					//Should be an OR/Single list since we have flattened before
					PDASSERT(subBooleanExp->mBooleanType == kBooleanOrList
						|| subBooleanExp->mBooleanType == kBooleanSingle);

					const BooleanList &subsubBooleanList = subBooleanExp->GetBooleanList();

					BooleanList::const_iterator subsubBooleanListIter = subsubBooleanList.begin();

					for (; subsubBooleanListIter != subsubBooleanList.end(); subsubBooleanListIter++)
					{
						Expr *newExpr = (*subsubBooleanListIter)->Clone();
						
						//Add the extra expr condition
						BooleanList newBooleanList;
						
						if (!needInitCurrentBooleanListList)
						{
							newBooleanList = *booleanListListIter;
						}
						newBooleanList.push_back(newExpr);
						newBooleanListList.push_back(newBooleanList);
					}
				}
				else
				{
					Expr *newExpr = (*booleanListIter)->Clone();
					
					//Add the extra expr condition
					BooleanList newBooleanList;

					if (!needInitCurrentBooleanListList)
					{
						newBooleanList = *booleanListListIter;
					}
					newBooleanList.push_back(newExpr);
					newBooleanListList.push_back(newBooleanList);
				}
				
				if (needInitCurrentBooleanListList)
				{
					needInitCurrentBooleanListList = false;
				}
				else
				{
					booleanListListIter++;
				}
			}

			currentBooleanListList = newBooleanListList;

			booleanListIter++;
		}

		PDASSERT(currentBooleanListList.size() > 0);

		if (currentBooleanListList.size() > 1)
		{
			CleanBooleanList_LocalStatic(&mBooleanList);
		
			//Build the final "Or" list containing all "And" lists
			for (std::vector<BooleanList>::const_iterator booleanListListIter = currentBooleanListList.begin()
				; booleanListListIter != currentBooleanListList.end()
				; booleanListListIter++)
			{
				BooleanExpr *subAndBooleanExpr = new BooleanExpr();
				subAndBooleanExpr->SetBooleanType(kBooleanAndList);

				for (BooleanList::const_iterator booleanListIter = booleanListListIter->begin()
					; booleanListIter != booleanListListIter->end()
					; booleanListIter++)
				{
					subAndBooleanExpr->AddExpr(*booleanListIter);
				}

				mBooleanList.push_back(subAndBooleanExpr);
			}

			this->SetBooleanType(kBooleanOrList);
		}
		else
		{
			//Turns out we don't improve anything
			//e.g A AND B
		}
	}
}

void BooleanExpr::Print(uint32_t level) const
{
	if (mBooleanType != kBooleanSingle)
	{
		this->PrintIndention(level);
		level++;
	}

	switch (mBooleanType)
	{
	case kBooleanSingle:
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

	switch (mBooleanType)
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
	case kBooleanSingle:
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

	switch (mBooleanType)
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
							multipleExpr->SetBooleanType(kBooleanAndList);

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
							multipleExpr->SetBooleanType(kBooleanOrList);

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
	case kBooleanSingle:
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

	result->mBooleanType = mBooleanType;
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