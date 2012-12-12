#include "stdafx.h"

#include "Optimizer/Plan/Planner.h"

#include "Optimizer/Join/JoinPath.h"

#include "Database/DBImpl.h"
#include "Database/Statement.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprWalker.h"

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

	//Generate Plan
	JoinInfoList joinInfoList;
	JoinPath joinPath;

	if (mPlanContext.mRelList.size() == 1)
	{
		joinPath.push_back(0);

		SeqScanNode *seqScanNode = new SeqScanNode(&mPlanContext, 0);
		newPlanNode = seqScanNode;
	}
	else if (mPlanContext.mRelList.size() > 1)
	{
		//Setup join order
		JoinInfo joinInfo;

		for (size_t i = 0; i < mPlanContext.mRelList.size(); i++)
		{
			joinPath.push_back(i);
			
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
		PlanNode *outerNode = new SeqScanNode(&mPlanContext, joinPath[0]);
		PlanNode *innerNode = new SeqScanNode(&mPlanContext, joinPath[1]);	

		for (size_t i = 2; i < joinPath.size(); i++)
		{
			outerNode = new NestLoopNode(&mPlanContext, joinInfo, outerNode, innerNode);
			innerNode = new SeqScanNode(&mPlanContext, joinPath[i]); 		
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
		Bitmask tableMask(joinPath.size());
		newPlanNode->SetupPredicate_Recursive(*mPlanContext.mpPredicateExpr, &tableMask);
	}

	newPlanNode->SetupProjection(mPlanContext.mRequiredColumns);

	newPlanNode->SetResultFunctor(&mPlanContext.mFinalResultFunctor);

	return newPlanNode;
}

}	// PandaSQL
