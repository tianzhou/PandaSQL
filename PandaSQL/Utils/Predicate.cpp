#include "stdafx.h"

#include "Utils/Predicate.h"

#include <iostream>

#include "Access/Tuple.h"

#include "Database/DBImpl.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
#include "Utils/Types.h"

namespace PandaSQL
{

//static Status EvalExprWithTupleContext(const std::vector<TupleEntry> &inTupleContext, const Expr &inExpr, std::string *io_str)
//{
//	Status result;
//
//	PDASSERT(inExpr.type == kExprColumnDef);
//
//	std::vector<TupleEntry>::const_iterator iter = inTupleContext.begin();
//
//	for (; iter != inTupleContext.end(); iter++)
//	{
//		if (iter->tableName == inExpr.columnDef.qualifiedName.tableName)
//		{
//			iter->tupleData.GetDataAtIndex(inExpr.columnDef.index, io_str);
//			break;
//		}
//	}
//
//	if (iter == inTupleContext.end())
//	{
//		result = Status::kInvalidExpression;
//	}
//
//	return result;
//}

/**************ColumnPredicate************/

TuplePredicate::TuplePredicate()
{

}

//void TuplePredicate::SetFormat(uint32_t index, ComparisonType type, const std::string value)
//{
//	mIndex = index;
//	mComparisonType = type;
//	mCompareValue = value;
//}

bool TuplePredicate::Eval(const TupleData &inTupleData) const
{
	bool result = false;

	//PDASSERT(mIndex < inTupleData.Count());

	//std::string tupleValue;

	//inTupleData.GetDataAtIndex(mIndex, &tupleValue);

	//switch (mComparisonType)
	//{
	//case kEqual:
	//	{
	//		result = tupleValue == mCompareValue;
	//		break;
	//	}
	//case kNotEqual:
	//	{
	//		result = tupleValue != mCompareValue;
	//		break;
	//	}

	//case kGreater:
	//	{
	//		result = tupleValue > mCompareValue;
	//		break;
	//	}

	//case kGreaterEqual:
	//	{
	//		result = tupleValue >= mCompareValue;
	//		break;
	//	}
	//case kLess:
	//	{
	//		result = tupleValue < mCompareValue;
	//		break;
	//	}
	//case kLessEqual:
	//	{
	//		result = tupleValue <= mCompareValue;
	//		break;
	//	}
	//default:
	//	PDASSERT(0);
	//	break;
	//}

	return result;
}

/**************PredicateItem**************/

PredicateItem::PredicateItem()
{
}

void PredicateItem::SetFormat(const Expr &inLeftExpr, const Expr &inRightExpr, PredicateComparisonType inPredicateType)
{
	mLeftExpr = inLeftExpr;
	mRightExpr = inRightExpr;
	mComparisonType = inPredicateType;
}

void PredicateItem::Print(uint32_t level) const
{
	std::string identation;

	for (uint32_t i = 0; i < level; i++)
	{
		identation += "  ";
	}

	std::string compStr;

	if (mComparisonType == kEqual)
	{
		compStr = "=";
	}
	else if (mComparisonType == kNotEqual)
	{
		compStr = "!=";
	}
	else if (mComparisonType == kGreater)
	{
		compStr = ">";
	}
	else if (mComparisonType == kGreaterEqual)
	{
		compStr = ">=";
	}
	else if (mComparisonType == kLess)
	{
		compStr = "<";
	}
	else if (mComparisonType == kLess)
	{
		compStr = "<=";
	}
	else
	{
		compStr = "Unknown";
	}

	std::cout << identation << compStr << std::endl;

	identation += "  ";

	//if (mLeftExpr.mType == kExprNumber)
	//{
	//	std::cout << identation << mLeftExpr.mNumberValue << std::endl;
	//}
	//else if (mLeftExpr.mType == kExprText)
	//{
	//	std::cout << identation << mLeftExpr.mTextValue << std::endl;
	//}
	//else if (mLeftExpr.mType == kExprColumnDef)
	//{
	//	std::cout << identation << mLeftExpr.mColumnDef.qualifiedName.tableName << "." << mLeftExpr.mColumnDef.qualifiedName.columnName << std::endl;
	//}

	//if (mRightExpr.mType == kExprNumber)
	//{
	//	std::cout << identation << mRightExpr.mNumberValue << std::endl;
	//}
	//else if (mRightExpr.mType == kExprText)
	//{
	//	std::cout << identation << mRightExpr.mTextValue << std::endl;
	//}
	//else if (mRightExpr.mType == kExprColumnDef)
	//{
	//	std::cout << identation << mRightExpr.mColumnDef.qualifiedName.tableName << "." << mRightExpr.mColumnDef.qualifiedName.columnName << std::endl;
	//}
}

Status PredicateItem::Prepare(const DBImpl &inDB, const Table::TableRefList &inTableRefList)
{
	Status result;

	//if (mLeftExpr.mType == kExprColumnDef)
	//{
	//	result = inDB.AmendColumnDef(inTableRefList, &mLeftExpr.mColumnDef);
	//}

	//if (result.OK())
	//{
	//	if (mRightExpr.mType == kExprColumnDef)
	//	{
	//		result = inDB.AmendColumnDef(inTableRefList, &mRightExpr.mColumnDef);
	//	}
	//}

	return result;
}

//bool PredicateItem::Eval(const std::vector<TupleEntry> &inTupleContext) const
//{
//	bool result = true;
//
//	std::string leftStrValue;
//	std::string rightStrValue;
//	uint32_t leftNumberValue;
//	uint32_t rightNumberValue;
//
//	DataType compareType;
//
//	if (mLeftExpr.type == kExprColumnDef)
//	{
//		compareType = mLeftExpr.columnDef.dataType;
//	}
//	else if (mRightExpr.type == kExprColumnDef)
//	{
//		compareType = mRightExpr.columnDef.dataType;
//	}
//	else
//	{
//		switch (mLeftExpr.type)
//		{
//		case kExprNumber:
//			{
//				compareType = kInt;
//				break;
//			}
//		case kExprText:
//			{
//				compareType = kText;
//				break;
//			}
//		default:
//			PDASSERT(0);
//			break;
//		}
//	}
//
//	if (mLeftExpr.type == kExprColumnDef)
//	{		
//		Status localResult = EvalExprWithTupleContext(inTupleContext, mLeftExpr, &leftStrValue);
//		
//		PDASSERT(localResult.OK());
//
//		if (compareType == kInt)
//		{
//			StringToNumber(leftStrValue, &leftNumberValue);
//		}
//	}
//	else if (mLeftExpr.type == kExprNumber)
//	{
//		leftNumberValue = mLeftExpr.number;
//
//		if (compareType == kText)
//		{
//			NumberToString(leftNumberValue, &leftStrValue);
//		}
//	}
//	else if (mLeftExpr.type == kExprText)
//	{
//		leftStrValue = mLeftExpr.text;
//
//		if (compareType == kInt)
//		{
//			StringToNumber(leftStrValue, &leftNumberValue);
//		}
//	}
//
//	if (mRightExpr.type == kExprColumnDef)
//	{	
//		Status localResult = EvalExprWithTupleContext(inTupleContext, mRightExpr, &rightStrValue);
//	
//		PDASSERT(localResult.OK());
//
//		if (compareType == kInt)
//		{
//			StringToNumber(rightStrValue, &rightNumberValue);
//		}
//	}
//	else if (mRightExpr.type == kExprNumber)
//	{
//		rightNumberValue = mRightExpr.number;
//
//		if (compareType == kText)
//		{
//			NumberToString(rightNumberValue, &rightStrValue);
//		}
//	}
//	else if (mRightExpr.type == kExprText)
//	{
//		rightStrValue = mRightExpr.text;
//
//		if (compareType == kInt)
//		{
//			StringToNumber(rightStrValue, &rightNumberValue);
//		}
//	}
//
//	switch (compareType)
//	{
//	case kInt:
//		{
//			switch (mComparisonType)
//			{
//			case kEqual:
//				{
//					result = leftNumberValue == rightNumberValue;
//					break;
//				}
//			case kNotEqual:
//				{
//					result = leftNumberValue != rightNumberValue;
//					break;
//				}
//
//			case kGreater:
//				{
//					result = leftNumberValue > rightNumberValue;
//					break;
//				}
//
//			case kGreaterEqual:
//				{
//					result = leftNumberValue >= rightNumberValue;
//					break;
//				}
//			case kLess:
//				{
//					result = leftNumberValue < rightNumberValue;
//					break;
//				}
//			case kLessEqual:
//				{
//					result = leftNumberValue <= rightNumberValue;
//					break;
//				}
//			default:
//				PDASSERT(0);
//				break;
//			}
//
//			break;
//		}
//	case kText:
//		{
//			switch (mComparisonType)
//			{
//			case kEqual:
//				{
//					result = leftStrValue == rightStrValue;
//					break;
//				}
//			case kNotEqual:
//				{
//					result = leftStrValue != rightStrValue;
//					break;
//				}
//
//			case kGreater:
//				{
//					result = leftStrValue > rightStrValue;
//					break;
//				}
//
//			case kGreaterEqual:
//				{
//					result = leftStrValue >= rightStrValue;
//					break;
//				}
//			case kLess:
//				{
//					result = leftStrValue < rightStrValue;
//					break;
//				}
//			case kLessEqual:
//				{
//					result = leftStrValue <= rightStrValue;
//					break;
//				}
//			default:
//				PDASSERT(0);
//				break;
//			}
//			break;
//		}
//	default:
//		PDASSERT(0);
//		break;
//	}
//	return result;
//}


/**************Predicate**************/

Predicate::Predicate()
:
mLogicGateType(kLogicUnknown)
{
}

Predicate::~Predicate()
{
}

void Predicate::SetSinglePredicateItem(const PredicateItem &inPredicateItem)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);

	mLogicGateType = kLogicStandalone;
	mPredicateItem = inPredicateItem;
}

void Predicate::SetAndPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);
	PDASSERT(inPredicateList.size() > 0);

	if (inPredicateList.size() == 1)
	{
		*this = inPredicateList[0];
	}
	else
	{
		mLogicGateType = kLogicAnd;
		mPredicateList = inPredicateList;
	}
}

void Predicate::SetOrPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);
	PDASSERT(inPredicateList.size() > 0);

	if (inPredicateList.size() == 1)
	{
		*this = inPredicateList[0];
	}
	else
	{
		mLogicGateType = kLogicOr;
		mPredicateList = inPredicateList;
	}
}

void Predicate::Reset()
{
	mLogicGateType = kLogicUnknown;

	mPredicateItem = PredicateItem();
	mPredicateList.clear();
}

void Predicate::Print(uint32_t level) const
{
	if (mLogicGateType != kLogicUnknown)
	{
		std::string identation;

		for (uint32_t i = 0; i < level; i++)
		{
			identation += "  ";
		}

		if (mLogicGateType == kLogicAnd
			|| mLogicGateType == kLogicOr)
		{
			if (mLogicGateType == kLogicAnd)
			{
				std::cout << identation << "And" << std::endl;
			}
			else
			{
				std::cout << identation << "Or" << std::endl;
			}

			identation += "  ";

			std::vector<Predicate>::const_iterator iter = mPredicateList.begin();

			for (; iter != mPredicateList.end(); iter++)
			{
				iter->Print(level + 1);
			}
		}
		else if (mLogicGateType == kLogicStandalone)
		{
			mPredicateItem.Print(level + 1);
		}
	}	
}

Status Predicate::Prepare(const DBImpl &inDB, const Table::TableRefList &inTableRefList)
{
	Status result;

	if (mLogicGateType == kLogicAnd
		|| mLogicGateType == kLogicOr)
	{
		std::vector<Predicate>::iterator iter = mPredicateList.begin();

		for (; iter != mPredicateList.end(); iter++)
		{
			result = iter->Prepare(inDB, inTableRefList);

			if (!result.OK())
			{
				break;
			}
		}
	}
	else if (mLogicGateType == kLogicStandalone)
	{
		result = mPredicateItem.Prepare(inDB, inTableRefList);
	}

	return result;
}

//bool Predicate::Eval(const std::vector<TupleEntry> &inTupleContext) const
//{
//	bool result = true;
//
//	if (mLogicGateType == kLogicAnd
//		|| mLogicGateType == kLogicOr)
//	{
//		std::vector<Predicate>::const_iterator iter = mPredicateList.begin();
//
//		for (; iter != mPredicateList.end(); iter++)
//		{
//			result = iter->Eval(inTupleContext);
//
//			if ((result && mLogicGateType == kLogicOr)
//				|| (!result && mLogicGateType == kLogicAnd))
//			{
//				break;
//			}
//		}
//	}
//	else if (mLogicGateType == kLogicStandalone)
//	{
//		result = mPredicateItem.Eval(inTupleContext);
//	}
//
//	return result;
//}

void Predicate::TransformToCNF()
{
	if (mLogicGateType == kLogicAnd
		|| mLogicGateType == kLogicOr)
	{
		std::vector<Predicate>::iterator iter = mPredicateList.begin();

		while (iter != mPredicateList.end())
		{
			iter->TransformToCNF();
			iter++;
		}

		if (mLogicGateType == kLogicOr)
		{
			PDASSERT(mPredicateList.size() >= 2);

			std::vector<Predicate>::iterator leftIter = mPredicateList.begin();
			std::vector<Predicate>::iterator rightIter = leftIter+1;
			std::vector<Predicate> newPredicateList;

			while (rightIter != mPredicateList.end())
			{
				//.......

				PDASSERT(leftIter->mLogicGateType == kLogicStandalone
					|| leftIter->mLogicGateType == kLogicAnd);

				PDASSERT(rightIter->mLogicGateType == kLogicStandalone
					|| rightIter->mLogicGateType == kLogicAnd);

				std::vector<Predicate> leftPredicateList;
				std::vector<Predicate> rightPredicateList;

				if (leftIter->mLogicGateType == kLogicStandalone)
				{
					leftPredicateList.push_back(*leftIter);
				}
				else
				{
					leftPredicateList = leftIter->mPredicateList;
				}

				if (rightIter->mLogicGateType == kLogicStandalone)
				{
					rightPredicateList.push_back(*rightIter);
				}
				else
				{
					rightPredicateList = rightIter->mPredicateList;
				}


				std::vector<Predicate>::iterator subLeftIter = leftPredicateList.begin();
				
				for (;subLeftIter != leftPredicateList.end();subLeftIter++)
				{
					std::vector<Predicate>::iterator subRightIter = rightPredicateList.begin();
					std::vector<Predicate> onePredicateList;
					
					//Push the left item
					onePredicateList.push_back(*subLeftIter);

					for (;subRightIter != rightPredicateList.end();subRightIter++)
					{
						//Push the right item
						onePredicateList.push_back(*subRightIter);
						
						Predicate onePredicate;
						onePredicate.SetOrPredicateWithSubpredicates(onePredicateList);
						
						newPredicateList.push_back(onePredicate);
						
						//Pop the right item
						onePredicateList.pop_back();
					}		
				}

				leftIter = rightIter;
				rightIter++;
			}
			
			this->Reset();

			this->SetAndPredicateWithSubpredicates(newPredicateList);
		}
	}
}

};	// PandaSQL