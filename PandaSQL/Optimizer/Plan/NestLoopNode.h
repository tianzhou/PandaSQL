#ifndef PANDASQL_NEST_LOOP_NODE_H
#define PANDASQL_NEST_LOOP_NODE_H

#include "PlanNode.h"
#include "PlanResult.h"
#include "Optimizer/Path/JoinPath.h"

namespace PandaSQL
{

class NestLoopNode;

class NestLoopResultFunctor : public PlanResultFunctor
{

public:

	NestLoopResultFunctor();

	virtual ~NestLoopResultFunctor();

	virtual void operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode);

private:
	
	friend class NestLoopNode;

	void SetNestLoopNode(NestLoopNode *io_nestLoopNode) { mpNestLoopNode = io_nestLoopNode; }

	NestLoopNode *mpNestLoopNode;
};

class NestLoopNode : public PlanNode
{

public:

	NestLoopNode(PlanContext *io_pPlanContext, const JoinInfo &inJoinInfo, PlanNode &inOuterNode, PlanNode &inInnerNode);

	const PlanNode& GetOuterNode() const;
	
	const PlanNode& GetInnerNode() const;

	virtual	void	Start();
	virtual	bool	Step();
	virtual	void	End();

private:

	bool MatchJoinPred();

	friend class NestLoopResultFunctor;

	const JoinInfo &mJoinInfo;
	NestLoopResultFunctor mNextLoopResultFunctor;
	PlanNode &mOuterNode;
	PlanNode &mInnerNode;
	ColumnDefList mOuterColumnDefList;	
	ColumnDefList mInnerColumnDefList;
	ValueList mOuterNodeCurrentValueList;
	ValueList mInnerNodeCurrentValueList;

	bool mNeedStepOuterNode;
};

}	// namespace PandaSQL

#endif	// PANDASQL_NEST_LOOP_NODE_H