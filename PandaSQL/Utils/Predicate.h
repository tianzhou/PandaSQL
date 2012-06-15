#ifndef PANDASQL_PREDICATE_H
#define PANDASQL_PREDICATE_H

#include "Catalog/Table.h"
#include "Utils/Expr.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class DB;

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

	void Print(uint32_t level) const;

	//inTableRefList: available table context
	Status Prepare(const DB &inDB, const Table::TableRefList &inTableRefList);

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

	void Print(uint32_t level) const;
	Status Prepare(const DB &inDB, const Table::TableRefList &inTableRefList);

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