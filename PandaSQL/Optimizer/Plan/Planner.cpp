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

	Table::TableRefList::const_iterator iter = allTableRef.begin();

	for(; iter != allTableRef.end(); iter++)
	{
		pDB->GetTableByName(*iter, &theTable);
		theRelNode = new RelNode(theTable);
		mPlanContext.mRelList.push_back(theRelNode);
	}

	if (mPlanContext.mRelList.size() == 1)
	{
		SeqScanNode *seqScanNode = new SeqScanNode(&mPlanContext, 0, NULL);

		newPlanNode = seqScanNode;
	}
	else if (mPlanContext.mRelList.size() > 1)
	{
		JoinPath theJoinPath;

		for (size_t i = 0; i < mPlanContext.mRelList.size(); i++)
		{
			theJoinPath.push_back(i);
		}

		PlanNode *innerNode = new SeqScanNode(&mPlanContext, theJoinPath[0], NULL);
		PlanNode *outerNode = new SeqScanNode(&mPlanContext, theJoinPath[1], NULL);

		for (size_t i = 2; i < theJoinPath.size(); i++)
		{
			innerNode = new NestLoopNode(&mPlanContext, *outerNode, *innerNode);
			outerNode = new SeqScanNode(&mPlanContext, theJoinPath[i], NULL); 		
		}

		newPlanNode = new NestLoopNode(&mPlanContext, *outerNode, *innerNode);
	}
	else
	{
		PDASSERT(0);
	}

	return newPlanNode;
}

}	// PandaSQL
