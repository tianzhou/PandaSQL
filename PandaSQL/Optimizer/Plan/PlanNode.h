#ifndef PANDASQL_PLAN_NODE_H
#define PANDASQL_PLAN_NODE_H

#include "Optimizer/Node.h"

#include "Optimizer/Plan/PlanContext.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class Bitmask;
class Expr;
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
	virtual void SetupPredicate_Recursive(const BooleanExpr &inPredicateExpr, Bitmask *io_tableMask);

	Status	GetLastStatus() const { return mLastStatus; }
	void SetResultFunctor(PlanResultFunctor *io_resultFunctor) { mpResultFunctor = io_resultFunctor; }

protected:

	mutable PlanContext *mpPlanContext;
	Status mLastStatus;

	Expr	*mpLocalPredicateExpr;
	PlanResultFunctor *mpResultFunctor;
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_NODE_H