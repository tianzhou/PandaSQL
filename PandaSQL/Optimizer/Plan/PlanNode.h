#ifndef PANDASQL_PLAN_NODE_H
#define PANDASQL_PLAN_NODE_H

#include "Optimizer/Node.h"

#include "Utils/Status.h"

namespace PandaSQL
{

struct PlanContext;

class PlanNode : public Node
{

public:

	PlanNode(NodeType inNodeType, PlanContext *io_pPlanContext);
	virtual ~PlanNode();

	virtual	void	Start();
	virtual	bool	Step();
	virtual	void	End();

	Status	GetLastStatus() const { return mLastStatus; }

protected:

	PlanContext *mpPlanContext;
	Status mLastStatus;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_NODE_H