#include "stdafx.h"

#include "PlanNode.h"

namespace PandaSQL
{

PlanNode::PlanNode(NodeType inNodeType, PlanContext *io_pPlanContext)
:
Node(inNodeType)
,mpPlanContext(io_pPlanContext)
{
}

PlanNode::~PlanNode()
{
}

void PlanNode::Start()
{

}

bool PlanNode::Step()
{
	return false;
}

void PlanNode::End()
{

}

}	// PandaSQL
