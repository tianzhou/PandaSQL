#include "stdafx.h"

#include "Predicate.h"
#include "DB.h"
#include "Access/Tuple.h"
#include "Utils/Types.h"
#include "Utils/Common.h"

#include <iostream>

namespace PandaSQL
{

static Status EvalExprWithTupleContext(const std::vector<TupleEntry> &inTupleContext, const Expr &inExpr, std::string *io_str)
{
	Status result;

	PDASSERT(inExpr.type == kExprColumnDef);

	std::vector<TupleEntry>::const_iterator iter = inTupleContext.begin();

	for (; iter != inTupleContext.end(); iter++)
	{
		if (iter->tableName == inExpr.columnDef.qualifiedName.tableName)
		{
			iter->tupleData.GetDataAtIndex(inExpr.columnDef.index, io_str);
			break;
		}
	}

	if (iter == inTupleContext.end())
	{
		result = Status::kInvalidExpression;
	}

	return result;
}

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

	PDASSERT(mIndex < inTupleData.Count());

	std::string tupleValue;

	inTupleData.GetDataAtIndex(mIndex, &tupleValue);

	switch (mComparisonType)
	{
	case kEqual:
		{
			result = tupleValue == mCompareValue;
			break;
		}
	case kNotEqual:
		{
			result = tupleValue != mCompareValue;
			break;
		}

	case kGreater:
		{
			result = tupleValue > mCompareValue;
			break;
		}

	case kGreaterEqual:
		{
			result = tupleValue >= mCompareValue;
			break;
		}
	case kLess:
		{
			result = tupleValue < mCompareValue;
			break;
		}
	case kLessEqual:
		{
			result = tupleValue <= mCompareValue;
			break;
		}
	default:
		PDASSERT(0);
		break;
	}

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

	if (mLeftExpr.type == kExprNumber)
	{
		std::cout << identation << mLeftExpr.number << std::endl;
	}
	else if (mLeftExpr.type == kExprText)
	{
		std::cout << identation << mLeftExpr.text << std::endl;
	}
	else if (mLeftExpr.type == kExprColumnDef)
	{
		std::cout << identation << mLeftExpr.columnDef.qualifiedName.tableName << "." << mLeftExpr.columnDef.qualifiedName.columnName << std::endl;
	}

	if (mRightExpr.type == kExprNumber)
	{
		std::cout << identation << mRightExpr.number << std::endl;
	}
	else if (mRightExpr.type == kExprText)
	{
		std::cout << identation << mRightExpr.text << std::endl;
	}
	else if (mRightExpr.type == kExprColumnDef)
	{
		std::cout << identation << mRightExpr.columnDef.qualifiedName.tableName << "." << mRightExpr.columnDef.qualifiedName.columnName << std::endl;
	}
}

Status PredicateItem::Prepare(const DB &inDB, const Table::TableRefList &inTableRefList)
{
	Status result;

	if (mLeftExpr.type == kExprColumnDef)
	{
		result = AmendColumnDef(inDB, inTableRefList, &mLeftExpr.columnDef);
	}

	if (result.OK())
	{
		if (mRightExpr.type == kExprColumnDef)
		{
			result = AmendColumnDef(inDB, inTableRefList, &mRightExpr.columnDef);
		}
	}

	return result;
}

bool PredicateItem::Eval(const std::vector<TupleEntry> &inTupleContext) const
{
	bool result = true;

	std::string leftStrValue;
	std::string rightStrValue;
	uint32_t leftNumberValue;
	uint32_t rightNumberValue;

	DataType compareType;

	if (mLeftExpr.type == kExprColumnDef)
	{
		compareType = mLeftExpr.columnDef.dataType;
	}
	else if (mRightExpr.type == kExprColumnDef)
	{
		compareType = mRightExpr.columnDef.dataType;
	}
	else
	{
		switch (mLeftExpr.type)
		{
		case kExprNumber:
			{
				compareType = kInt;
				break;
			}
		case kExprText:
			{
				compareType = kText;
				break;
			}
		default:
			PDASSERT(0);
			break;
		}
	}

	if (mLeftExpr.type == kExprColumnDef)
	{		
		Status localResult = EvalExprWithTupleContext(inTupleContext, mLeftExpr, &leftStrValue);
		
		PDASSERT(localResult.OK());

		if (compareType == kInt)
		{
			StringToNumber(leftStrValue, &leftNumberValue);
		}
	}
	else if (mLeftExpr.type == kExprNumber)
	{
		leftNumberValue = mLeftExpr.number;

		if (compareType == kText)
		{
			NumberToString(leftNumberValue, &leftStrValue);
		}
	}
	else if (mLeftExpr.type == kExprText)
	{
		leftStrValue = mLeftExpr.text;

		if (compareType == kInt)
		{
			StringToNumber(leftStrValue, &leftNumberValue);
		}
	}

	if (mRightExpr.type == kExprColumnDef)
	{	
		Status localResult = EvalExprWithTupleContext(inTupleContext, mRightExpr, &rightStrValue);
	
		PDASSERT(localResult.OK());

		if (compareType == kInt)
		{
			StringToNumber(rightStrValue, &rightNumberValue);
		}
	}
	else if (mRightExpr.type == kExprNumber)
	{
		rightNumberValue = mRightExpr.number;

		if (compareType == kText)
		{
			NumberToString(rightNumberValue, &rightStrValue);
		}
	}
	else if (mRightExpr.type == kExprText)
	{
		rightStrValue = mRightExpr.text;

		if (compareType == kInt)
		{
			StringToNumber(rightStrValue, &rightNumberValue);
		}
	}

	switch (compareType)
	{
	case kInt:
		{
			switch (mComparisonType)
			{
			case kEqual:
				{
					result = leftNumberValue == rightNumberValue;
					break;
				}
			case kNotEqual:
				{
					result = leftNumberValue != rightNumberValue;
					break;
				}

			case kGreater:
				{
					result = leftNumberValue > rightNumberValue;
					break;
				}

			case kGreaterEqual:
				{
					result = leftNumberValue >= rightNumberValue;
					break;
				}
			case kLess:
				{
					result = leftNumberValue < rightNumberValue;
					break;
				}
			case kLessEqual:
				{
					result = leftNumberValue <= rightNumberValue;
					break;
				}
			default:
				PDASSERT(0);
				break;
			}

			break;
		}
	case kText:
		{
			switch (mComparisonType)
			{
			case kEqual:
				{
					result = leftStrValue == rightStrValue;
					break;
				}
			case kNotEqual:
				{
					result = leftStrValue != rightStrValue;
					break;
				}

			case kGreater:
				{
					result = leftStrValue > rightStrValue;
					break;
				}

			case kGreaterEqual:
				{
					result = leftStrValue >= rightStrValue;
					break;
				}
			case kLess:
				{
					result = leftStrValue < rightStrValue;
					break;
				}
			case kLessEqual:
				{
					result = leftStrValue <= rightStrValue;
					break;
				}
			default:
				PDASSERT(0);
				break;
			}
			break;
		}
	default:
		PDASSERT(0);
		break;
	}
	return result;
}


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

void Predicate::SetSinglePredicate(const Predicate &inPredicate)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);

	mLogicGateType = kLogicStandalone;

	mSinglePredicate = new Predicate(inPredicate);
}

void Predicate::SetAndPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);

	mLogicGateType = kLogicAnd;
	mPredicateList = inPredicateList;
}

void Predicate::SetOrPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);

	mLogicGateType = kLogicOr;
	mPredicateList = inPredicateList;
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

Status Predicate::Prepare(const DB &inDB, const Table::TableRefList &inTableRefList)
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

bool Predicate::Eval(const std::vector<TupleEntry> &inTupleContext) const
{
	bool result = true;

	if (mLogicGateType == kLogicAnd
		|| mLogicGateType == kLogicOr)
	{
		std::vector<Predicate>::const_iterator iter = mPredicateList.begin();

		for (; iter != mPredicateList.end(); iter++)
		{
			result = iter->Eval(inTupleContext);

			if ((result && mLogicGateType == kLogicOr)
				|| (!result && mLogicGateType == kLogicAnd))
			{
				break;
			}
		}
	}
	else if (mLogicGateType == kLogicStandalone)
	{
		result = mPredicateItem.Eval(inTupleContext);
	}

	return result;
}

};	// PandaSQL