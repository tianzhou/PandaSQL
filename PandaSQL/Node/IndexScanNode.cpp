#include "stdafx.h"

#include "Node/IndexScanNode.h"

#include "Database/DBImpl.h"

#include "Optimizer/Plan/PlanContext.h"


namespace PandaSQL
{

IndexScanNode::IndexScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex)
:
ScanNode(kNodeIndexScan, io_pPlanContext, inRelIndex)
{
}

IndexScanNode::~IndexScanNode()
{
}

TupleIterator* IndexScanNode::CreateScanIterator(const Table *pTable)
{
	return mpPlanContext->mpDB->CreateSeqScanIteratorForTable(*pTable, mTupleDesc);
}

}	// PandaSQL
