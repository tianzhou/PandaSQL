#ifndef PANDASQL_PLAN_CONTEXT_H
#define PANDASQL_PLAN_CONTEXT_H

#include <vector>

namespace PandaSQL
{

class RelNode;

struct PlanContext
{
	std::vector<const RelNode*> mRelList;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_CONTEXT_H