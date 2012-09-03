#include "stdafx.h"

#include "ExprContext.h"

#include "Access/Iterator.h"

#include "Catalog/Column.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

void ExprContext::UpdateColumnWithTupleData(const ColumnQualifiedName &inColumnQualifiedName, const Value &inValue)
{
	PDASSERT(mColumnValueMap.find(inColumnQualifiedName) == mColumnValueMap.end());

	mColumnValueMap.insert(ColumnValuePair(inColumnQualifiedName, inValue));
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