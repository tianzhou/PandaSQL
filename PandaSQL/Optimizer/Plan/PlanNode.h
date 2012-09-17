#ifndef PANDASQL_PLAN_NODE_H
#define PANDASQL_PLAN_NODE_H

#include "Optimizer/Node.h"

#include "Utils/Status.h"

namespace PandaSQL
{

struct PlanContext;
class PlanResultFunctor;

class PlanNode : public Node
{

public:

	PlanNode(NodeType inNodeType, PlanContext *io_pPlanContext);
	virtual ~PlanNode();

	virtual	void	Start();
	virtual	bool	Step();
	virtual	void	End();

	Status	GetLastStatus() const { return mLastStatus; }
	void SetResultFunctor(PlanResultFunctor *io_resultFunctor) { mpResultFunctor = io_resultFunctor; }

protected:

	PlanContext *mpPlanContext;
	Status mLastStatus;

	PlanResultFunctor *mpResultFunctor;

};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_NODE_H