#include "stdafx.h"

#include "Node/PlanNode.h"

namespace PandaSQL
{

PlanNode::PlanNode(NodeType inNodeType, PlanContext *io_pPlanContext)
:
Node(inNodeType)
,mpPlanContext(io_pPlanContext)
,mpLocalPredicateExpr(NULL)
{
}

PlanNode::~PlanNode()
{
	delete mpLocalPredicateExpr;
	mpLocalPredicateExpr = NULL;
}

void PlanNode::Reset()
{

}

bool PlanNode::Step()
{
	return false;
}

void PlanNode::End()
{

}

void PlanNode::SetupProjection(const TableAndColumnSetMap &inRequiredColumns)
{
}

void PlanNode::SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask)
{
}

}	// PandaSQL
