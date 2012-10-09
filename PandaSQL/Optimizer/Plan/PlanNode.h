#ifndef PANDASQL_PLAN_NODE_H
#define PANDASQL_PLAN_NODE_H

#include "Optimizer/Node.h"

#include "Optimizer/Plan/PlanContext.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class PlanResultFunctor;

class PlanNode : public Node
{

public:

	PlanNode(NodeType inNodeType, PlanContext *io_pPlanContext);
	virtual ~PlanNode();

	virtual	void	Reset();
	virtual	bool	Step();
	virtual	void	End();

	//Recursively setup projection
	virtual void SetupProjection(const TableAndColumnSetMap &inRequiredColumns);

	Status	GetLastStatus() const { return mLastStatus; }
	void SetResultFunctor(PlanResultFunctor *io_resultFunctor) { mpResultFunctor = io_resultFunctor; }

protected:

	PlanContext *mpPlanContext;
	Status mLastStatus;

	PlanResultFunctor *mpResultFunctor;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_NODE_H