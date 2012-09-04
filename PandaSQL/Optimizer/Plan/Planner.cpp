#include "stdafx.h"

#include "Planner.h"

#include "PlanNode.h"
#include "NestLoopNode.h"
#include "SeqScanNode.h"

#include "Optimizer/Path/JoinPath.h"

#include "Parser/Statement.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

Planner::Planner(const Statement &inStatement)
:
mStatement(inStatement)
{
}

PlanNode* Planner::GeneratePlan()
{
	PlanNode *newPlanNode = NULL;

	const Table::TableRefList &joinList = mStatement.GetTableRefList();

	if (joinList.size() == 1)
	{
		SeqScanNode *seqScanNode = new SeqScanNode();


		newPlanNode = seqScanNode;
	}
	else if (joinList.size() > 1)
	{
		JoinPath *joinPath = new JoinPath();

		NestLoopNode *nestLoopNode = new NestLoopNode(*joinPath);

		newPlanNode = nestLoopNode;
	}
	else
	{
		Table::TableRefList::const_iterator iter = joinList.begin();
		PDASSERT(0);
	}

	return newPlanNode;
}

}	// PandaSQL
