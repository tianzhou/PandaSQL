#include "stdafx.h"

#include "Predicate.h"
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

};	// PandaSQL