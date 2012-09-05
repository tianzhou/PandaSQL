#include "stdafx.h"

#include "SeqScanNode.h"

#include "Optimizer/RelNode.h"

namespace PandaSQL
{

SeqScanNode::SeqScanNode(PlanContext *inPlanContext, uint32_t inRelIndex)
:
PlanNode(kNodeSeqScan, inPlanContext)
,mRelIndex(inRelIndex)
{
}

void SeqScanNode::ExecutePlan() const
{
}


}	// PandaSQL
