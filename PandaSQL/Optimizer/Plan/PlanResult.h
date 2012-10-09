#ifndef PANDASQL_PLAN_RESULT_H
#define PANDASQL_PLAN_RESULT_H

#include "Catalog/Column.h"

#include "Utils/Status.h"
#include "Utils/Value.h"

namespace PandaSQL
{

class PlanNode;

class PlanResultFunctor
{

public:

	PlanResultFunctor() {}

	virtual ~PlanResultFunctor() = 0 {}

	virtual void operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode) = 0;

private:

};

class FinalResultFunctor : public PlanResultFunctor
{

public:

	FinalResultFunctor();

	virtual ~FinalResultFunctor();

	virtual void operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode);

	void SetProjectionList(const ColumnDefList &inResultProjectionList);

private:

	const ColumnDefList *mpResultProjectionList;
	
};

}	// namespace PandaSQL

#endif	// PANDASQL_PLAN_RESULT_H
