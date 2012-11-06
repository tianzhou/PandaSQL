#include "stdafx.h"

#include "Executor/Executor.h"

#include "Node/PlanNode.h"

namespace PandaSQL
{

Status Executor::ExecutePlan(PlanNode *io_pRootPlanNode)
{
	Status result;
	
	while(io_pRootPlanNode->Step());

	io_pRootPlanNode->End();

	return io_pRootPlanNode->GetLastStatus();
}

}	// PandaSQL
