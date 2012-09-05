#ifndef PANDASQL_NEST_LOOP_NODE_H
#define PANDASQL_NEST_LOOP_NODE_H

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

class NestLoopNode : public PlanNode
{

public:

	NestLoopNode(PlanContext *inPlanContext, const PlanNode &inOuterNode, const PlanNode &inInnerNode);

	const PlanNode& GetOuterNode() const;
	
	const PlanNode& GetInnerNode() const;

	virtual void ExecutePlan() const;

private:

	const PlanNode &mOuterNode;
	const PlanNode &mInnerNode;
};

}	// namespace PandaSQL

#endif	// PANDASQL_NEST_LOOP_NODE_H