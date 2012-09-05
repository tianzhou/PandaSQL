#include "stdafx.h"

#include "PlanNode.h"

namespace PandaSQL
{

PlanNode::PlanNode(NodeType inNodeType, PlanContext *inPlanContext)
:
Node(inNodeType)
,mpPlanContext(inPlanContext)
{
}


void PlanNode::ExecutePlan() const
{
}


}	// PandaSQL
