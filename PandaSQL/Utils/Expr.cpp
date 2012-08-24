#include "stdafx.h"

#include "Expr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

void Expr::Eval(TupleDescElement inDescElement, TupleDataElement *io_data) const
{
	//TODO: No coercing at this point
	if (inDescElement.mDataType == kInt)
	{
		io_data->mNumber = mNumberValue;
	}
	else if (inDescElement.mDataType == kText)
	{
		io_data->mText = mTextValue;
	}
	else
	{
		//TODO
		PDASSERT(0);
	}
}

void Expr::EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, TupleData *io_tupleData)
{
	PDASSERT(inExprList.size() == inTupleDesc.size());

	ExprList::const_iterator exprIter = inExprList.begin();
	TupleDesc::const_iterator tupleDescIter = inTupleDesc.begin();

	while (exprIter != inExprList.end())
	{
		TupleDataElement oneTupleElement;

		exprIter->Eval(*tupleDescIter, &oneTupleElement);

		io_tupleData->push_back(oneTupleElement);

		exprIter++;
		tupleDescIter++;
	}
}

}	// PandaSQL
