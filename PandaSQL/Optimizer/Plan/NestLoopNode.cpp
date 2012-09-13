#include "stdafx.h"

#include "NestLoopNode.h"

#include "Optimizer/Plan/PlanContext.h"

namespace PandaSQL
{

NestLoopNode::NestLoopNode(PlanContext *inPlanContext, PlanNode &inOuterNode, PlanNode &inInnerNode)
:
PlanNode(kNodeNestLoop, inPlanContext)
,mOuterNode(inOuterNode)
,mInnerNode(inInnerNode)
,mNeedStepOuterNode(false)
{
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

	bool result = true;

	if (mNeedStepOuterNode)
	{
		if (!mOuterNode.Step())
		{
			result = false;
		}
		
		mNeedStepOuterNode = false;
	}

	if (result)
	{
		if (!mOuterNode.Step())
		{
			mNeedStepOuterNode = true;
		}
	}

	return result;
}

void NestLoopNode::End()
{
}

}	// PandaSQL
