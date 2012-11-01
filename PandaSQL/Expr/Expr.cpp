#include "stdafx.h"

#include "Expr/Expr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

Expr::Expr()
:
mExprType(kExprUnknown)
{
}

Expr::Expr(ExprType inExprType)
:
mExprType(inExprType)
{
}

Expr::~Expr()
{
	mExprType = kExprUnknown;
}

void Expr::Eval(TupleDescElement inDescElement, Value *io_data) const
{
	//TODO: No coercing at this point
	if (inDescElement.mDataType == kInt)
	{
		io_data->SetAsNumber(mNumberValue);
	}
	else if (inDescElement.mDataType == kText)
	{
		io_data->SetAsString(mTextValue);
	}
	else
	{
		//TODO
		PDASSERT(0);
	}
}

void Expr::EvalExprList(const ExprList &inExprList, const TupleDesc &inTupleDesc, ValueList *io_tupleValueList)
{
	PDASSERT(inExprList.size() == inTupleDesc.size());

	ExprList::const_iterator exprIter = inExprList.begin();
	TupleDesc::const_iterator tupleDescIter = inTupleDesc.begin();

	while (exprIter != inExprList.end())
	{
		Value oneValue;

		exprIter->Eval(*tupleDescIter, &oneValue);

		io_tupleValueList->push_back(oneValue);

		exprIter++;
		tupleDescIter++;
	}
}

bool Expr::IsTrue(const ExprContext &inExprContext) const
{
	return true;
}

Status Expr::GetValue(const ExprContext &inExprContext, Value *io_value) const
{
	Status result;

	return result;
}

Expr* Expr::CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const
{
	//TODO: Should never get called if we exclude this before doing pushdown
	return this->Clone();
}

void Expr::Walk(ExprWalker *io_walker) const
{

}

void Expr::MutableWalk(MutableExprWalker *io_walker)
{

}

Expr* Expr::Clone() const
{
	Expr *result = new Expr(this->GetType());

	return result;
}

}	// PandaSQL
