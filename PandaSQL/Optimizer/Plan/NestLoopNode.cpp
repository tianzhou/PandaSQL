#include "stdafx.h"

#include "NestLoopNode.h"

#include "PlanContext.h"

#include "Utils/Debug.h"
#include "Utils/Expr/BooleanExpr.h"
#include "Utils/Expr/ExprContext.h"

#include <algorithm>

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
				if (MatchJoinPred())
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

void NestLoopNode::SetupProjection(const TableAndColumnSetMap &inRequiredColumns)
{
	mpOuterNode->SetupProjection(inRequiredColumns);
	mpInnerNode->SetupProjection(inRequiredColumns);
}

bool NestLoopNode::MatchJoinPred()
{
	ExprContext exprContext;
	exprContext.UpdateTupleValue(mOuterColumnDefList, mOuterNodeCurrentValueList);
	exprContext.UpdateTupleValue(mInnerColumnDefList, mInnerNodeCurrentValueList);
	
	return mpPlanContext->mpPredicateExpr->IsTrue(&exprContext);
}

}	// PandaSQL
