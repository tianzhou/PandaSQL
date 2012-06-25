#ifndef PANDASQL_PREDICATE_H
#define PANDASQL_PREDICATE_H

#include "Catalog/Table.h"
#include "Utils/Expr.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class DB;
class Tuple;

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

	bool Eval(const Tuple *tuple1 = NULL, const Tuple *tuple2 = NULL) const;

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
	

	//If both side are constant, both tuple1 & tuple2 must be NULL. tuple1 is assigned to the left side while tuple2 is assigned to the right side
	//If neither side is constant, both tuple1 & tuple2 must be non NULL.
	//If only one side is constant, tuple1 must be not-NULL while tuple2 must be NULL.
	bool Eval(const Tuple *tuple1 = NULL, const Tuple *tuple2 = NULL) const;

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