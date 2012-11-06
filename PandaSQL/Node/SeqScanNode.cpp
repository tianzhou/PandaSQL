#include "stdafx.h"

#include "Node/SeqScanNode.h"

#include <algorithm>

#include "Node/RelNode.h"

#include "Access/Tuple.h"
#include "Access/TupleIterator.h"

#include "Catalog/Table.h"

#include "Database/DBImpl.h"

#include "Expr/BooleanExpr.h"

#include "Optimizer/Plan/PlanContext.h"
#include "Optimizer/Plan/PlanResult.h"

#include "Utils/Bitmask.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

SeqScanNode::SeqScanNode(PlanContext *io_pPlanContext, uint32_t inRelIndex)
:
PlanNode(kNodeSeqScan, io_pPlanContext)
,mRelIndex(inRelIndex)
,mpTupleIterator(NULL)
{
	const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
	const Table *pTable = pRelNode->GetTable();

	ColumnDefListToTupleDesc(pTable->GetAllColumns(), &mTupleDesc); 

	mpTupleIterator = mpPlanContext->mpDB->CreateTupleIteratorForTable(*pTable, mTupleDesc);

	mLastStatus = mpTupleIterator->GetLastError();
}

SeqScanNode::~SeqScanNode()
{
	PDASSERT(!mpTupleIterator);
}

void SeqScanNode::Reset()
{
	PDASSERT(mpTupleIterator);
	
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

	while (mpTupleIterator->Next())
	{
		ValueList theValueList;
		if (mpTupleIterator->GetValue(&theValueList))
		{
			const RelNode *pRelNode = mpPlanContext->mRelList[mRelIndex];
			const Table *pTable = pRelNode->GetTable();
			
			mpPlanContext->mExprContext.UpdateTupleValue(pTable->GetAllColumns(), theValueList);

			//If no applicable predicate for this node at this point(may be evaluated on higher level)
			//OR predicate evals to true
			if (!mpLocalPredicateExpr || mpLocalPredicateExpr->IsTrue(mpPlanContext->mExprContext))
			{
				(*mpResultFunctor)(pTable->GetAllColumns(), theValueList, *this);
				result = true;
				break;
			}		
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

void SeqScanNode::SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask)
{
	//The leaf node shouldn't have any table mask set
	PDASSERT(io_tableMask->IsClear());

	io_tableMask->SetBit(mRelIndex, true);

	std::vector<std::string> tableNameList;

	for (uint32_t i = 0; i < io_tableMask->GetLength(); i++)
	{
		if (io_tableMask->GetBit(i))
		{
			const RelNode *pRelNode = mpPlanContext->mRelList[i];
			const Table *pTable = pRelNode->GetTable();

			tableNameList.push_back(pTable->GetName());
		}
	}

	mpLocalPredicateExpr = inPredicateExpr.CreateSubExprForPushdown(tableNameList);
}

}	// PandaSQL
