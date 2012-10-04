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

void NestLoopNode::Start()
{
	mpOuterNode->Start();
	mLastStatus = mpOuterNode->GetLastStatus();

	if (mLastStatus.OK())
	{
		mpInnerNode->Start();
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
		}

		while (!mNeedStepOuterNode && !hasNext)
		{
			if (mpInnerNode->Step())
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
		}
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
