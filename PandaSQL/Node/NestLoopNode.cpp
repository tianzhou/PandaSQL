#include "stdafx.h"

#include "Node/NestLoopNode.h"
#include "Node/RelNode.h"

#include <algorithm>

#include "Catalog/Table.h"

#include "Expr/BooleanExpr.h"
#include "Expr/ExprContext.h"

#include "Optimizer/Plan/PlanContext.h"

#include "Utils/Bitmask.h"
#include "Utils/Debug.h"

namespace PandaSQL
{

NestLoopResultFunctor::NestLoopResultFunctor()
:
mpNestLoopNode(NULL)
{
}

NestLoopResultFunctor::~NestLoopResultFunctor()
{
}

void
NestLoopResultFunctor::operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode)
{
	//mpPlanContext->mExprContext.UpdateTupleValue(inColumnList, inValueList);

	if (&inPlanNode == &(mpNestLoopNode->GetOuterNode()))
	{
		mpNestLoopNode->mOuterNodeCurrentValueList = inValueList;
		mpNestLoopNode->mOuterColumnDefList = inColumnList;
	}
	else if (&inPlanNode == &(mpNestLoopNode->GetInnerNode()))
	{
		mpNestLoopNode->mInnerNodeCurrentValueList = inValueList;
		mpNestLoopNode->mInnerColumnDefList = inColumnList;
	}
	else
	{
		PDASSERT(0);
	}
}

NestLoopNode::NestLoopNode(PlanContext *io_pPlanContext, const JoinInfo &inJoinInfo, PlanNode *io_outerNode, PlanNode *io_innerNode)
:
PlanNode(kNodeNestLoop, io_pPlanContext)
,mJoinInfo(inJoinInfo)
,mNestLoopResultFunctor()
,mpOuterNode(io_outerNode)
,mpInnerNode(io_innerNode)
,mNeedStepOuterNode(false)
{
	mNestLoopResultFunctor.SetNestLoopNode(this);
	mpOuterNode->SetResultFunctor(&mNestLoopResultFunctor);
	mpInnerNode->SetResultFunctor(&mNestLoopResultFunctor);
}

NestLoopNode::~NestLoopNode()
{
}

const PlanNode& NestLoopNode::GetOuterNode() const
{
	return *mpOuterNode;
}
	
const PlanNode& NestLoopNode::GetInnerNode() const
{
	return *mpInnerNode;
}

void NestLoopNode::Reset()
{
	mpOuterNode->Reset();
	mLastStatus = mpOuterNode->GetLastStatus();

	if (mLastStatus.OK())
	{
		mpInnerNode->Reset();
		mLastStatus = mpInnerNode->GetLastStatus();

		if (mLastStatus.OK()) 
		{
			mNeedStepOuterNode = true;
		}
	}
}

bool NestLoopNode::Step()
{
	if (!mLastStatus.OK())
	{
		return false;
	}

	bool hasNext = false;

	while (!hasNext)
	{
		if (mNeedStepOuterNode)
		{
			if (!mpOuterNode->Step())
			{
				break;
			} 

			mNeedStepOuterNode = false;

			mpInnerNode->Reset();
		}

		while (!mNeedStepOuterNode && !hasNext)
		{
			if (mpInnerNode->Step())
			{
				if (this->MatchPredicate())
				{
					//TODO: This is quite in-efficient
					ColumnDefList theColumnDefList = mOuterColumnDefList;
					theColumnDefList.insert(theColumnDefList.end(), mInnerColumnDefList.begin(), mInnerColumnDefList.end());
					
					ValueList theValueList = mOuterNodeCurrentValueList;
					theValueList.insert(theValueList.end(), mInnerNodeCurrentValueList.begin(), mInnerNodeCurrentValueList.end());

					(*mpResultFunctor)(theColumnDefList, theValueList, *this);
				
					hasNext = true;
				}
			}
			else
			{
				mNeedStepOuterNode = true;
			}
		}
	}

	return hasNext;
}

void NestLoopNode::End()
{
}

bool NestLoopNode::MatchPredicate() const
{
	mpPlanContext->mExprContext.UpdateTupleValue(mOuterColumnDefList, mOuterNodeCurrentValueList);
	mpPlanContext->mExprContext.UpdateTupleValue(mInnerColumnDefList, mInnerNodeCurrentValueList);
	return !mpLocalPredicateExpr
		|| mpLocalPredicateExpr->IsTrue(mpPlanContext->mExprContext);
}

void NestLoopNode::SetupProjection(const TableAndColumnSetMap &inRequiredColumns)
{
	mpOuterNode->SetupProjection(inRequiredColumns);
	mpInnerNode->SetupProjection(inRequiredColumns);
}

void NestLoopNode::SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask)
{
	Bitmask outerMask(io_tableMask->GetLength());
	mpOuterNode->SetupPredicate_Recursive(inPredicateExpr, &outerMask);

	Bitmask innerMask(io_tableMask->GetLength());
	mpInnerNode->SetupPredicate_Recursive(inPredicateExpr, &innerMask);

	io_tableMask->Union(outerMask, innerMask);

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
