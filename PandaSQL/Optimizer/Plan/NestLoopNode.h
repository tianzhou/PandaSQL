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

	//Take the ownership of outer and inner node
	NestLoopNode(PlanContext *io_pPlanContext, const JoinInfo &inJoinInfo, PlanNode *io_outerNode, PlanNode *io_innerNode);

	virtual ~NestLoopNode();

	const PlanNode& GetOuterNode() const;
	
	const PlanNode& GetInnerNode() const;

	virtual	void	Reset();
	virtual	bool	Step();
	virtual	void	End();

	virtual void	SetupProjection(const TableAndColumnSetMap &inRequiredColumns);
	virtual void	SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

protected:


private:

	bool MatchPredicate() const;

	friend class NestLoopResultFunctor;

	const JoinInfo &mJoinInfo;
	NestLoopResultFunctor mNestLoopResultFunctor;
	PlanNode *mpOuterNode;
	PlanNode *mpInnerNode;
	ColumnDefList mOuterColumnDefList;	
	ColumnDefList mInnerColumnDefList;
	ValueList mOuterNodeCurrentValueList;
	ValueList mInnerNodeCurrentValueList;

	bool mNeedStepOuterNode;
};

}	// namespace PandaSQL

#endif	// PANDASQL_NEST_LOOP_NODE_H