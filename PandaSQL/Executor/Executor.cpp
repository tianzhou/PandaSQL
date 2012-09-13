#include "stdafx.h"

#include "Executor.h"

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

Status Executor::ExecutePlan(PlanNode *io_pRootPlanNode)
{
	Status result;

	io_pRootPlanNode->Start();
	
	while(io_pRootPlanNode->Step());

	io_pRootPlanNode->End();

	return io_pRootPlanNode->GetLastStatus();
}

}	// PandaSQL
