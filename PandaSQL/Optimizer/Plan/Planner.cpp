#include "stdafx.h"

#include "Optimizer/Plan/Planner.h"

#include "Optimizer/Join/JoinPath.h"

#include "Database/DBImpl.h"
#include "Database/Statement.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprWalker.h"

#include "Node/IndexScanNode.h"
#include "Node/NestLoopNode.h"
#include "Node/PlanNode.h"
#include "Node/RelNode.h"
#include "Node/SeqScanNode.h"

#include "Utils/Bitmask.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

//Although one can get PandaDB from inStatement, we want to get the non-const version PandaDB
Planner::Planner(const Statement &inStatement, DBImpl *io_dbImpl)
:
mStatement(inStatement)
{
	mPlanContext.mpDB = io_dbImpl;
}

PlanNode* Planner::GeneratePlan()
{
	PlanNode *newPlanNode = NULL;

	const DBImpl *pDB = mStatement.GetDB();
	const Table *theTable = NULL;
	const RelNode *theRelNode = NULL;

	const Table::TableRefList &allTableRef = mStatement.GetTableRefList();
	const ColumnDefList &targetColumnDefList = mStatement.GetTargetColumnDefList();
	const BooleanExpr *whereExpr = mStatement.GetWhereExpr();
	
	//Setup plan predicate expression
	mPlanContext.mpPredicateExpr = whereExpr;

	//Setup final projection list
	mPlanContext.mFinalResultFunctor.SetProjectionList(targetColumnDefList);

	//Populate base relation list
	Table::TableRefList::const_iterator iter = allTableRef.begin();
	for(; iter != allTableRef.end(); iter++)
	{
		pDB->GetTableByName(*iter, &theTable);
		theRelNode = new RelNode(theTable);
		mPlanContext.mRelList.push_back(theRelNode);
	}

	std::map<std::string, const Index*> candidateIndexListForTable;

	//Find appropriate index, whereExpr should be in CNF now
	//e.g. (A AND C) OR (A AND D) OR (B AND C)
	if (whereExpr)
	{
		const BooleanExpr::BooleanList &whereList = whereExpr->GetBooleanList();

		BooleanExpr::BooleanList::const_iterator whereIter = whereList.begin();

		while (whereIter != whereList.end())
		{
			TableAndColumnSetMap requiredColumnsForTable;
			DependentColumnListWalker walker(&requiredColumnsForTable);

			(*whereIter)->Walk(&walker);

			TableAndColumnSetMap::const_iterator tableColumnIter = requiredColumnsForTable.begin();

			while (tableColumnIter != requiredColumnsForTable.end())
			{
				ColumnNameSet columnNameSet = tableColumnIter->second;

				const Index *pIndex = NULL;
				pDB->GetIndexByTableColumns(tableColumnIter->first, columnNameSet, &pIndex);

				if (pIndex)
				{
					candidateIndexListForTable[tableColumnIter->first] = pIndex;
				}

				tableColumnIter++;
			}

			whereIter++;
		}
	}

	//Generate Plan
	JoinInfoList joinInfoList;
	JoinPath joinPath;

	if (mPlanContext.mRelList.size() == 1)
	{
		joinPath.push_back(0);

		newPlanNode = this->CreateScanNode_Private(0, candidateIndexListForTable);	
	}
	else if (mPlanContext.mRelList.size() > 1)
	{
		//Setup join order and join type

		//TODO: Join order is fixed and we haven't setup
		//any join info yet
		JoinInfo joinInfo;

		for (size_t i = 0; i < mPlanContext.mRelList.size(); i++)
		{
			joinPath.push_back((uint32_t)i);
		}

		// Construct left deep tree
		//		A
		//	   / \
		//    /\  \
		//   B  C  D
		//  /\ 
		// E  F
		PlanNode *outerNode = this->CreateScanNode_Private(joinPath[0], candidateIndexListForTable);
		PlanNode *innerNode = this->CreateScanNode_Private(joinPath[1], candidateIndexListForTable);

		for (size_t i = 2; i < joinPath.size(); i++)
		{
			outerNode = new NestLoopNode(&mPlanContext, joinInfo, outerNode, innerNode);
			innerNode = this->CreateScanNode_Private(joinPath[i], candidateIndexListForTable); 		
		}

		newPlanNode = new NestLoopNode(&mPlanContext, joinInfo, outerNode, innerNode);
	}
	else
	{
		PDASSERT(0);
	}

	//Setup all required columns
	ColumnDefList::const_iterator final_projection_iter = targetColumnDefList.begin();
	for (; final_projection_iter != targetColumnDefList.end(); final_projection_iter++)
	{
		AddOneColumnToMap(final_projection_iter->qualifiedName, &mPlanContext.mRequiredColumns);	
	}

	if (mPlanContext.mpPredicateExpr)
	{
		DependentColumnListWalker walker(&mPlanContext.mRequiredColumns);
		mPlanContext.mpPredicateExpr->Walk(&walker);

		//Initial mask is clear, which means no table info is available
		//We populate the mask bottom-up as we encounter base relations
		Bitmask tableMask(joinPath.size());
		newPlanNode->SetupPredicate_Recursive(*mPlanContext.mpPredicateExpr, &tableMask);
	}

	newPlanNode->SetupProjection(mPlanContext.mRequiredColumns);

	newPlanNode->SetResultFunctor(&mPlanContext.mFinalResultFunctor);

	return newPlanNode;
}

ScanNode* Planner::CreateScanNode_Private(uint32_t inBaseRelIndex, const std::map<std::string, const Index*> inCandidateIndexListForTable)
{
	ScanNode *scanNode = NULL;

	const RelNode *pRelNode = mPlanContext.mRelList[inBaseRelIndex];
	const Table *pTable = pRelNode->GetTable();

	std::map<std::string, const Index*>::const_iterator iter = inCandidateIndexListForTable.find(pTable->GetName());

	if (iter == inCandidateIndexListForTable.end())
	{
		scanNode = new SeqScanNode(&mPlanContext, inBaseRelIndex);
	}
	else
	{
		scanNode = new IndexScanNode(&mPlanContext, inBaseRelIndex, *iter->second);
	}

	return scanNode;
}

}	// PandaSQL
