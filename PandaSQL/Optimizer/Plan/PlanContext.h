#ifndef PANDASQL_PLAN_CONTEXT_H
#define PANDASQL_PLAN_CONTEXT_H

#include "Optimizer/Plan/PlanResult.h"

#include <vector>

#include "Catalog/Column.h"

#include "Expr/ExprContext.h"

namespace PandaSQL
{

class RelNode;
class DBImpl;
class BooleanExpr;

typedef std::vector<uint32_t> ProjectionIndexList;
typedef std::vector<uint32_t> PredicateIndexList;

struct PlanContext
{
	std::vector<const RelNode*> mRelList;
	DBImpl *mpDB;
	const BooleanExpr *mpPredicateExpr;
	FinalResultFunctor mFinalResultFunctor;

	//For expression evaluation
	ExprContext mExprContext;

	//For select
	TableAndColumnSetMap mRequiredColumns;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_CONTEXT_H