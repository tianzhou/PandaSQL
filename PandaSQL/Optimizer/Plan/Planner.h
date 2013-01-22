#ifndef PANDASQL_PLANNER_H
#define PANDASQL_PLANNER_H

#include "Optimizer/Plan/PlanContext.h"

namespace PandaSQL
{

class DBImpl;
class Index;
class PlanNode;
class ScanNode;
class Statement;

class Planner
{

public:

	Planner(const Statement &inStatement, DBImpl *io_dbImpl);

	PlanNode* GeneratePlan();

private:

	ScanNode* CreateScanNode_Private(int32_t inRelIndex, const std::map<std::string, const Index*> inCandidateIndexListForTable);

	const Statement &mStatement;
	PlanContext mPlanContext;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLANNER_H