#ifndef PANDASQL_PLAN_CONTEXT_H
#define PANDASQL_PLAN_CONTEXT_H

#include <vector>

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
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_CONTEXT_H