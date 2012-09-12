#include "stdafx.h"

#include "NestLoopNode.h"

#include "Optimizer/Plan/PlanContext.h"

namespace PandaSQL
{

NestLoopNode::NestLoopNode(PlanContext *inPlanContext, const PlanNode &inOuterNode, const PlanNode &inInnerNode)
:
PlanNode(kNodeNestLoop, inPlanContext)
,mOuterNode(inOuterNode)
,mInnerNode(inInnerNode)
{
}

const PlanNode& NestLoopNode::GetOuterNode() const
{
	return mOuterNode;
}
	
const PlanNode& NestLoopNode::GetInnerNode() const
{
	return mInnerNode;
}

}	// PandaSQL
