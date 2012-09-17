#include "stdafx.h"

#include "NestLoopNode.h"

#include "PlanContext.h"

#include "Utils/Debug.h"
#include "Utils/Expr/BooleanExpr.h"
#include "Utils/Expr/ExprContext.h"

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

NestLoopNode::NestLoopNode(PlanContext *io_pPlanContext, const JoinInfo &inJoinInfo, PlanNode &inOuterNode, PlanNode &inInnerNode)
:
PlanNode(kNodeNestLoop, io_pPlanContext)
,mJoinInfo(inJoinInfo)
,mNextLoopResultFunctor()
,mOuterNode(inOuterNode)
,mInnerNode(inInnerNode)
,mNeedStepOuterNode(false)
{
	mNextLoopResultFunctor.SetNestLoopNode(this);
	mOuterNode.SetResultFunctor(&mNextLoopResultFunctor);
	mInnerNode.SetResultFunctor(&mNextLoopResultFunctor);
}

const PlanNode& NestLoopNode::GetOuterNode() const
{
	return mOuterNode;
}
	
const PlanNode& NestLoopNode::GetInnerNode() const
{
	return mInnerNode;
}

void NestLoopNode::Start()
{
	mOuterNode.Start();
	mLastStatus = mOuterNode.GetLastStatus();

	if (mLastStatus.OK())
	{
		mInnerNode.Start();
		mLastStatus = mInnerNode.GetLastStatus();

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

	while (1)
	{
		if (mNeedStepOuterNode)
		{
			if (!mOuterNode.Step())
			{
				break;
			} 

			mNeedStepOuterNode = false;
		}

		do
		{
			if (mInnerNode.Step())
			{
				if (MatchJoinPred())
			 	{
					ValueList theList;
					ColumnDefList theColumnDefList;
					
					(*mpResultFunctor)(theColumnDefList, theList, *this);
					hasNext = true;
				}
			}
			else
			{
				mNeedStepOuterNode = true;
			}
		}while (hasNext || mNeedStepOuterNode);
	}

	return hasNext;
}

void NestLoopNode::End()
{
}

bool NestLoopNode::MatchJoinPred()
{
	bool result = true;
	const BooleanExpr::BooleanList &predList = mpPlanContext->mpPredicateExpr->GetBooleanList();

	ExprContext exprContext;
	exprContext.UpdateTupleValue(mOuterColumnDefList, mOuterNodeCurrentValueList);
	exprContext.UpdateTupleValue(mInnerColumnDefList, mInnerNodeCurrentValueList);
	for (size_t i = 0; i < mJoinInfo.mPredicateIndexList.size(); i++)
	{
		if (!predList[mJoinInfo.mPredicateIndexList[i]]->IsTrue(&exprContext))
		{
			result = false;
			break;
		}
	}

	return result;
}

}	// PandaSQL
