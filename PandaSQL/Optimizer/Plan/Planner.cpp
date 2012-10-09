#include "stdafx.h"

#include "Planner.h"

#include "PlanNode.h"
#include "NestLoopNode.h"
#include "SeqScanNode.h"

#include "PandaDB.h"

#include "Optimizer/Path/JoinPath.h"
#include "Optimizer/RelNode.h"

#include "Parser/Statement.h"

#include "Utils/Debug.h"
#include "Utils/Expr/BooleanExpr.h"

namespace PandaSQL
{

//Although one can get PandaDB from inStatement, we want to get the non-const version PandaDB
Planner::Planner(const Statement &inStatement, PandaDB *io_pPandaDB)
:
mStatement(inStatement)
{
	mPlanContext.mpDB = io_pPandaDB;
}

PlanNode* Planner::GeneratePlan()
{
	PlanNode *newPlanNode = NULL;

	const PandaDB *pDB = mStatement.GetDB();
	Table *theTable = NULL;
	const RelNode *theRelNode = NULL;

	const Table::TableRefList &allTableRef = mStatement.GetTableRefList();
	const ColumnDefList &targetColumnDefList = mStatement.GetTargetColumnDefList();
	const BooleanExpr *whereExpr = mStatement.GetWhereExpr();
	mPlanContext.mpPredicateExpr = whereExpr;

	//TODO: The second one might be unnecessary.
	mPlanContext.mFinalResultFunctor.SetProjectionList(targetColumnDefList);	
	mPlanContext.mpFinalProjectionList = &targetColumnDefList;

	Table::TableRefList::const_iterator iter = allTableRef.begin();

	for(; iter != allTableRef.end(); iter++)
	{
		pDB->GetTableByName(*iter, &theTable);
		theRelNode = new RelNode(theTable);
		mPlanContext.mRelList.push_back(theRelNode);
	}

	JoinInfoList joinInfoList;
	if (mPlanContext.mRelList.size() == 1)
	{
		SeqScanNode *seqScanNode = new SeqScanNode(&mPlanContext, 0);
		newPlanNode = seqScanNode;
	}
	else if (mPlanContext.mRelList.size() > 1)
	{
		JoinPath theJoinPath;
		JoinInfo joinInfo;

		for (size_t i = 0; i < mPlanContext.mRelList.size(); i++)
		{
			theJoinPath.push_back(i);
			
			//if (i > 0)
			//{
			//	GenerateJoinInfo(
			//		*mPlanContext.mRelList[i-1]
			//		, *mPlanContext.mRelList[i]
			//		, *mPlanContext.mpPredicateExpr
			//		, &joinInfo);

			//	joinInfoList.push_back(joinInfo);
			//}
		}

		// Construct left deep tree
		//		A
		//	   / \
		//    /\  \
		//   B  C  D
		//  /\ 
		// E  F
		PlanNode *outerNode = new SeqScanNode(&mPlanContext, theJoinPath[0]);
		PlanNode *innerNode = new SeqScanNode(&mPlanContext, theJoinPath[1]);	

		for (size_t i = 2; i < theJoinPath.size(); i++)
		{
			outerNode = new NestLoopNode(&mPlanContext, joinInfo, outerNode, innerNode);
			innerNode = new SeqScanNode(&mPlanContext, theJoinPath[i]); 		
		}

		newPlanNode = new NestLoopNode(&mPlanContext, joinInfo, outerNode, innerNode);
	}
	else
	{
		PDASSERT(0);
	}

	ProjectionIndexList rootProjectionList;
	for (size_t i = 0; i < mPlanContext.mpFinalProjectionList->size(); i++)
	{
		rootProjectionList.push_back(i);
	}

	ColumnDefList::const_iterator final_projection_iter = mPlanContext.mpFinalProjectionList->begin();

	for (; final_projection_iter != mPlanContext.mpFinalProjectionList->end(); final_projection_iter++)
	{
		AddOneColumnToMap(final_projection_iter->qualifiedName, &mPlanContext.mRequiredColumns);	
	}

	mPlanContext.mpPredicateExpr->GetDependentColumns(&mPlanContext.mRequiredColumns);
	

	PredicateIndexList rootPredicateList;
	if (mPlanContext.mpPredicateExpr)
	{
		for (size_t i = 0; i < mPlanContext.mpPredicateExpr->GetBooleanList().size(); i++)
		{
			rootPredicateList.push_back(i);
		}
	}

	newPlanNode->SetupProjection(mPlanContext.mRequiredColumns);

	newPlanNode->SetResultFunctor(&mPlanContext.mFinalResultFunctor);

	return newPlanNode;
}

}	// PandaSQL
