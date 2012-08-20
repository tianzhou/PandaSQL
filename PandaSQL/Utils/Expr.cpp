#include "stdafx.h"

#include "Expr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

void Eval(const ExprList &inExprList, ColumnValueList *io_valueList)
{
	ExprList::const_iterator iter = inExprList.begin();

	//TODO: No coercing at this point
	for (; iter != inExprList.end(); iter++)
	{
		ColumnValue oneColumnValue;

		if (iter->type == kExprNumber)
		{
			oneColumnValue.valueType = kInt;
			oneColumnValue.number = iter->number;
		}
		else if (iter->type == kExprText)
		{
			oneColumnValue.valueType = kText;
			oneColumnValue.text = iter->text;
		}
		else if (iter->type == kExprColumnDef)
		{
			//TODO
			PDASSERT(0);
		}

		io_valueList->push_back(oneColumnValue);
	}
}

}	// PandaSQL
