#ifndef PANDASQL_EXECUTOR_H
#define PANDASQL_EXECUTOR_H

#include "Utils/Status.h"

namespace PandaSQL
{

class PlanNode;

class Executor
{

public:

	Status ExecutePlan(const PlanNode &inPlanNode);

private:


};

}	// namespace PandaSQL

#endif	// PANDASQL_EXECUTOR_H