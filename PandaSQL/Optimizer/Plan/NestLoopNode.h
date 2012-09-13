#ifndef PANDASQL_NEST_LOOP_NODE_H
#define PANDASQL_NEST_LOOP_NODE_H

#include "Optimizer/Plan/PlanNode.h"

namespace PandaSQL
{

class NestLoopNode : public PlanNode
{

public:

	NestLoopNode(PlanContext *inPlanContext, PlanNode &inOuterNode, PlanNode &inInnerNode);

	const PlanNode& GetOuterNode() const;
	
	const PlanNode& GetInnerNode() const;

	virtual	void	Start();
	virtual	bool	Step();
	virtual	void	End();

private:

	PlanNode &mOuterNode;
	PlanNode &mInnerNode;
	bool mNeedStepOuterNode;
};

}	// namespace PandaSQL

#endif	// PANDASQL_NEST_LOOP_NODE_H