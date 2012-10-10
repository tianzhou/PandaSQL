#ifndef PANDASQL_PLAN_CONTEXT_H
#define PANDASQL_PLAN_CONTEXT_H

#include "PlanResult.h"

#include "Catalog/Column.h"

#include "Utils/Expr/ExprContext.h"

#include <vector>

namespace PandaSQL
{

class RelNode;
class PandaDB;
class BooleanExpr;

typedef std::vector<uint32_t> ProjectionIndexList;
typedef std::vector<uint32_t> PredicateIndexList;

struct PlanContext
{
	std::vector<const RelNode*> mRelList;
	PandaDB *mpDB;
	const BooleanExpr *mpPredicateExpr;
	FinalResultFunctor mFinalResultFunctor;

	//For expression evaluation
	ExprContext mExprContext;

	//For select
	TableAndColumnSetMap mRequiredColumns;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_CONTEXT_H