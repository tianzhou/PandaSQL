#ifndef PANDASQL_PLANNER_H
#define PANDASQL_PLANNER_H

#include "Optimizer/Plan/PlanContext.h"

namespace PandaSQL
{

class DBImpl;
class PlanNode;
class Statement;

class Planner
{

public:

	Planner(const Statement &inStatement, DBImpl *io_dbImpl);

	PlanNode* GeneratePlan();

private:

	const Statement &mStatement;
	PlanContext mPlanContext;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLANNER_H