#include "stdafx.h"

#include "Expr/Expr.h"

#include "Expr/ExprContext.h"

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

Status Expr::EvalExprList(const ExprList &inExprList, const ExprContext &inExprContext, ValueList *io_tupleValueList)
{
	Status result;

	ExprList::const_iterator exprIter = inExprList.begin();

	while (exprIter != inExprList.end())
	{
		Value oneValue;

		result = (*exprIter)->Eval(inExprContext, &oneValue);

		if (!result.OK())
		{
			break;
		}

		io_tupleValueList->push_back(oneValue);

		exprIter++;
	}

	return result;
}

bool Expr::IsTrue(const ExprContext &inExprContext) const
{
	return true;
}

Status Expr::Eval(const ExprContext &inExprContext, Value *io_value) const
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

void Expr::Print(uint32_t level) const
{
}

void Expr::PrintIndention(uint32_t level) const
{
	while (level-- > 0)
	{
		std::cout << "  ";
	}
}

}	// PandaSQL
