#include "stdafx.h"

#include "Node/SeqScanNode.h"

#include "Database/DBImpl.h"

#include "Optimizer/Plan/PlanContext.h"

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

TupleIterator* SeqScanNode::CreateScanIterator(const Table *pTable)
{
	return mpPlanContext->mpDB->CreateSeqScanIteratorForTable(*pTable, mTupleDesc);
}

}	// PandaSQL
