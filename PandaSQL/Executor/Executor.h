#ifndef PANDASQL_EXECUTOR_H
#define PANDASQL_EXECUTOR_H

#include "Utils/Status.h"

namespace PandaSQL
{

class PlanNode;

class Executor
{

public:

	Status ExecutePlan(PlanNode *io_pRootPlanNode);

private:

};

}	// namespace PandaSQL

#endif	// PANDASQL_EXECUTOR_H