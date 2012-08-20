#ifndef PANDASQL_PREDICATE_H
#define PANDASQL_PREDICATE_H

#include "Catalog/Table.h"
#include "Access/Tuple.h"
#include "Utils/Expr.h"
#include "Utils/Types.h"

#include <vector>

namespace PandaSQL
{

class PandaDB;

class TuplePredicate
{
public:

	enum ComparisonType
	{
		kUnknown,
		kEqual,
		kNotEqual,
		kGreater,
		kGreaterEqual,
		kLess,
		kLessEqual,		
	};

	TuplePredicate();
	//void SetFormat(uint32_t index, ComparisonType type, const std::string value);
	bool Eval(const TupleData &inTupleData) const;

private:

	uint32_t mIndex;
	ComparisonType mComparisonType;
	std::string mCompareValue;

};

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
	Status Prepare(const PandaDB &inDB, const Table::TableRefList &inTableRefList);
	
	bool Eval(const std::vector<TupleEntry> &inTupleContext) const;

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
	void SetAndPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList);
	void SetOrPredicateWithSubpredicates(const std::vector<Predicate> &inPredicateList);
	void Reset();

	void Print(uint32_t level) const;
	Status Prepare(const PandaDB &inDB, const Table::TableRefList &inTableRefList);
	void TransformToCNF(); 

	bool Eval(const std::vector<TupleEntry> &inTupleContext) const;

private:

	enum PredicateLogicGateType
	{
		kLogicUnknown,
		kLogicStandalone,
		kLogicAnd,
		kLogicOr,
	};

privileged:

	PredicateLogicGateType mLogicGateType;

	std::vector<Predicate> mPredicateList;
	PredicateItem mPredicateItem;
};

}	// PandaSQL

#endif	// PANDASQL_PREDICATE_H