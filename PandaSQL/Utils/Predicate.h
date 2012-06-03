#ifndef PANDASQL_PREDICATE_H
#define PANDASQL_PREDICATE_H

#include "Utils/Expr.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class PredicateItem
{
public:
	
	enum PredicateComparisonType
	{
		kUnknown,
		kEqual,
		kNotEqual,
		kGreater,
		kGreaterEqual,
		kLess,
		kLessEqual,		
	};

	PredicateItem();
	void SetFormat(const Expr &inLeftExpr, const Expr &inRightExpr, PredicateComparisonType inPredicateType);

private:

	Expr mLeftExpr;
	Expr mRightExpr;
	PredicateComparisonType mComparisonType;

};

class Predicate
{
public:

	Predicate();
	~Predicate();

	void SetSinglePredicateItem(const PredicateItem &inPredicateItem);
	void SetAndPredicateWithSubpredicates(const std::vector<Predicate> inPredicateList);
	void SetOrPredicateWithSubpredicates(const std::vector<Predicate> inPredicateList);
	void Reset();

private:
	enum PredicateLogicGateType
	{
		kLogicUnknown,
		kLogicStandalone,
		kLogicAnd,
		kLogicOr,
	};

	PredicateLogicGateType mLogicGateType;

	std::vector<Predicate> mPredicateList;
	PredicateItem mPredicateItem;
};

}	// PandaSQL

#endif	// PANDASQL_PREDICATE_H