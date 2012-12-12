#include "stdafx.h"

#include "Node/SeqScanNode.h"

namespace PandaSQL
{

SeqScanNode::SeqScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex)
:
ScanNode(kNodeSeqScan, io_pPlanContext, inRelIndex)
{
}

SeqScanNode::~SeqScanNode()
{
}

}	// PandaSQL
