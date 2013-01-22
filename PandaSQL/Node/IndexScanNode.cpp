#include "stdafx.h"

#include "Node/IndexScanNode.h"

#include "Node/RelNode.h"

#include "Database/DBImpl.h"

#include "Optimizer/Plan/PlanContext.h"


namespace PandaSQL
{

IndexScanNode::IndexScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex, const Index &inIndex)
:
ScanNode(kNodeIndexScan, io_pPlanContext, inRelIndex)
,mIndex(inIndex)
{
}

IndexScanNode::~IndexScanNode()
{
}

TupleIterator* IndexScanNode::CreateScanIterator()
{
	TupleIterator *result = NULL;

	if (mLastStatus.OK())
	{
		result = mpPlanContext->mpDB->CreateIndexScanIteratorForTable(mIndex, mTupleDesc, mpLocalPredicateExpr);
	}

	return result;
}

}	// PandaSQL
