#include "stdafx.h"

#include "Node/SeqScanNode.h"

#include "Node/RelNode.h"

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

TupleIterator* SeqScanNode::CreateScanIterator()
{
	const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
	const Table *pTable = pRelNode->GetTable();

	return mpPlanContext->mpDB->CreateSeqScanIteratorForTable(*pTable, mTupleDesc);
}

}	// PandaSQL
