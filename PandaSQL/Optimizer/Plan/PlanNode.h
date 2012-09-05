#ifndef PANDASQL_PLAN_NODE_H
#define PANDASQL_PLAN_NODE_H

#include "Optimizer/Node.h"

namespace PandaSQL
{

struct PlanContext;

class PlanNode : public Node
{

public:

	PlanNode(NodeType inNodeType, PlanContext *inPlanContext);

	virtual void ExecutePlan() const;

protected:

	PlanContext *mpPlanContext;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_NODE_H