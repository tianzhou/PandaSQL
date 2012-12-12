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
	TupleIterator *result = NULL;
	std::vector<const Index *> indexList;

	mLastStatus = mpPlanContext->mpDB->GetAllIndexesByTableName(pTable->GetName(), &indexList);

	if (mLastStatus.OK())
	{
		//TODO: Fetch most appropriate index
		result = mpPlanContext->mpDB->CreateIndexScanIteratorForTable(*indexList[0], mTupleDesc, mpLocalPredicateExpr);
	}

	return result;
}

}	// PandaSQL
