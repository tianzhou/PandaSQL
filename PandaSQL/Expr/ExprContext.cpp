#include "stdafx.h"

#include "Expr/ExprContext.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

void ExprContext::UpdateTupleValue(const ColumnDefList &inColumnDefList, const ValueList &inValueList)
{
	PDASSERT(inColumnDefList.size() == inValueList.size());

	ColumnDefList::const_iterator iter = inColumnDefList.begin();
	ValueList::const_iterator iter2 = inValueList.begin();

	while (iter != inColumnDefList.end())
	{
		mColumnValueMap[iter->qualifiedName] = *iter2;
	
		iter++;
		iter2++;
	}	
}

void ExprContext::GetColumnValue(const ColumnQualifiedName &inColumnQualifiedName, Value *io_value) const
{
	ColumnValueMap::const_iterator iter = mColumnValueMap.find(inColumnQualifiedName);

	if (iter != mColumnValueMap.end())
	{
		*io_value = iter->second;
	}
	else
	{
		*io_value = Value();
	}
}

}	// PandaSQL