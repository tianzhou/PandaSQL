#include "stdafx.h"

#include "PlanResult.h"

namespace PandaSQL
{

FinalResultFunctor::FinalResultFunctor()
{
}

FinalResultFunctor::~FinalResultFunctor()
{
}

void
FinalResultFunctor::operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode)
{

}

}	// PandaSQL
