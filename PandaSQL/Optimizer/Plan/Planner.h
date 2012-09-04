#ifndef PANDASQL_PLANNER_H
#define PANDASQL_PLANNER_H

namespace PandaSQL
{

class PlanNode;
class Statement;

class Planner
{

public:

	Planner(const Statement &inStatement);

	PlanNode* GeneratePlan();

private:

	const Statement &mStatement;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLANNER_H