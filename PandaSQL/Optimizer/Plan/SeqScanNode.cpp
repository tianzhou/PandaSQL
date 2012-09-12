#include "stdafx.h"

#include "SeqScanNode.h"

#include "PlanContext.h"

#include "PandaDB.h"

#include "Optimizer/RelNode.h"

#include "Access/Tuple.h"
#include "Access/TupleIterator.h"

#include "Catalog/Table.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

TupleFunctor::TupleFunctor()
{
}

void TupleFunctor::operator()(const ValueList &inValueList)
{
#ifdef PDDEBUG
	PrintTuple(inValueList);
#endif
}

SeqScanNode::SeqScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex, TupleFunctor *io_pTupleFunctor)
:
PlanNode(kNodeSeqScan, io_pPlanContext)
,mRelIndex(inRelIndex)
,mpTupleFunctor(io_pTupleFunctor)
,mpTupleIterator(NULL)
{
	PDASSERT(mpTupleFunctor);
}

SeqScanNode::~SeqScanNode()
{
	PDASSERT(!mpTupleIterator);
}

void SeqScanNode::Start()
{
	PDASSERT(mRelIndex >= 0 && mRelIndex < mpPlanContext->mRelList.size());

	if (!mpTupleIterator)
	{
		const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
		const Table *pTable = pRelNode->GetTable();

		mpTupleIterator = mpPlanContext->mpDB->CreateTupleIteratorForTable(*pTable);
	}
	
	mpTupleIterator->Reset();

	mLastStatus = mpTupleIterator->GetLastError();
}

bool SeqScanNode::Step()
{
	if (!mLastStatus.OK())
	{
		return false;
	}

	bool result = false;

	PDASSERT(mpTupleIterator);

	if (mpTupleIterator->Next())
	{
		ValueList theValueList;
		if (mpTupleIterator->GetValue(&theValueList))
		{
			(*mpTupleFunctor)(theValueList);
			result = true;
		}
	}

	mLastStatus = mpTupleIterator->GetLastError();

	return result;
}

void SeqScanNode::End()
{
	PDASSERT(mpTupleIterator);

	delete mpTupleIterator;
	mpTupleIterator = NULL;
}

}	// PandaSQL
