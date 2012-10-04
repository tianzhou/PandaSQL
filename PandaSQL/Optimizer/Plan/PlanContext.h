#ifndef PANDASQL_PLAN_CONTEXT_H
#define PANDASQL_PLAN_CONTEXT_H

#include <vector>

#include "PlanResult.h"

namespace PandaSQL
{

class RelNode;
class PandaDB;
class BooleanExpr;

struct PlanContext
{
	std::vector<const RelNode*> mRelList;
	PandaDB *mpDB;
	const BooleanExpr *mpPredicateExpr;
	FinalResultFunctor mFinalResultFunctor;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_CONTEXT_H