#include "stdafx.h"

#include "Predicate.h"
#include "DB.h"
#include "Utils/Types.h"

#include <iostream>

namespace PandaSQL
{

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

Status PredicateItem::Prepare(const DB &inDB, const Table::TableRefList inTableRefList)
{
	Status result;

	if (mLeftExpr.type == kExprColumnDef)
	{
		if (mLeftExpr.columnDef.qualifiedName.tableName.empty())
		{
			Table::TableRefList::const_iterator iter = inTableRefList.begin();

			for (; iter != inTableRefList.end(); iter++)
			{
				Table *theTable;

				result = inDB.GetTableByName(*iter, &theTable);

				if (result.OK())
				{
					Status localStatus = theTable->GetColumnByName(mLeftExpr.columnDef.qualifiedName.columnName, &mLeftExpr.columnDef);

					if (localStatus.OK())
					{
						mLeftExpr.columnDef.qualifiedName.tableName = theTable->GetName();
						break;
					}
				}
			}

			if (iter == inTableRefList.end())
			{
				result = Status::kColumnMissing;
			}
		}
		else
		{
			Table *theTable;
			result = inDB.GetTableByName(mLeftExpr.columnDef.qualifiedName.tableName, &theTable);
		}
	}

	if (result.OK())
	{
		if (mRightExpr.type == kExprColumnDef)
		{
			mRightExpr.columnDef.qualifiedName.tableName = "yyy";
		}
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

void Predicate::SetAndPredicateWithSubpredicates(const std::vector<Predicate> inPredicateList)
{
	//Make sure it's not assigned before. Otherwise, it's likely a code error
	PDASSERT(mLogicGateType == kLogicUnknown);

	mLogicGateType = kLogicAnd;
	mPredicateList = inPredicateList;
}

void Predicate::SetOrPredicateWithSubpredicates(const std::vector<Predicate> inPredicateList)
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

Status Predicate::Prepare(const DB &inDB, const Table::TableRefList inTableRefList)
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

};	// PandaSQL