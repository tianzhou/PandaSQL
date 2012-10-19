#include "stdafx.h"

#include "Optimizer/Plan/PlanResult.h"

#include "Access/Tuple.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

FinalResultFunctor::FinalResultFunctor()
:
mpResultProjectionList(NULL)
{
}

FinalResultFunctor::~FinalResultFunctor()
{
}

void
FinalResultFunctor::operator()(const ColumnDefList &inColumnList, const ValueList &inValueList, const PlanNode &inPlanNode)
{
	ValueList resultValueList;

	ColumnDefList::const_iterator result_proj_iter = mpResultProjectionList->begin();

	size_t index;
	for (; result_proj_iter != mpResultProjectionList->end(); result_proj_iter++)
	{
		ColumnDefList::const_iterator in_col_iter = inColumnList.begin();

		index = 0;
		for (; in_col_iter != inColumnList.end(); in_col_iter++)
		{
			if (*result_proj_iter == *in_col_iter)
			{
				resultValueList.push_back(inValueList[index]);
				break;
			}
			index++;
		}

		//We must find a matching column in the result projection list
		PDASSERT(in_col_iter != inColumnList.end());
	}

#ifdef PDDEBUG
	PrintTuple(resultValueList);
#endif
}

void
FinalResultFunctor::SetProjectionList(const ColumnDefList &inResultProjectionList)
{
	mpResultProjectionList = &inResultProjectionList;
}

}	// PandaSQL
