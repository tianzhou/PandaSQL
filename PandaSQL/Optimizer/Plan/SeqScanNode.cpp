#include "stdafx.h"

#include "SeqScanNode.h"

#include "PlanContext.h"
#include "PlanResult.h"

#include "PandaDB.h"

#include "Optimizer/RelNode.h"

#include "Access/Tuple.h"
#include "Access/TupleIterator.h"

#include "Catalog/Table.h"

#include "Utils/Debug.h"

#include <algorithm>

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

SeqScanNode::SeqScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex)
:
PlanNode(kNodeSeqScan, io_pPlanContext)
,mRelIndex(inRelIndex)
,mpTupleIterator(NULL)
{
}

SeqScanNode::~SeqScanNode()
{
	PDASSERT(!mpTupleIterator);
}

void SeqScanNode::Reset()
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
			const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
			const Table *pTable = pRelNode->GetTable();
			(*mpResultFunctor)(pTable->GetAllColumns(), theValueList, *this);
			mpPlanContext->mExprContext.UpdateTupleValue(pTable->GetAllColumns(), theValueList);
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

void SeqScanNode::SetupProjection(const TableAndColumnSetMap &inRequiredColumns)
{
	PDASSERT(mProjectionList.empty());

	const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
	const Table *pTable = pRelNode->GetTable();
	TableAndColumnSetMap::const_iterator table_column_map_iter = inRequiredColumns.find(pTable->GetName());

	if (table_column_map_iter != inRequiredColumns.end())
	{
		const ColumnDefList &allColumns = pTable->GetAllColumns();
	
		ColumnDefList::const_iterator columnIter = allColumns.begin();

		for (; columnIter != allColumns.end(); columnIter++)
		{
			if (table_column_map_iter->second.find(columnIter->qualifiedName.columnName) != table_column_map_iter->second.end())
			{
				mProjectionList.push_back(columnIter->index);
			}
		}
	}

	std::sort(mProjectionList.begin(), mProjectionList.end());
}

}	// PandaSQL
