#include "stdafx.h"

#include "Executor.h"

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

Status Executor::ExecutePlan(const PlanNode &inPlanNode)
{
	Status result;

	inPlanNode.ExecutePlan();

	return result;
}

}	// PandaSQL
