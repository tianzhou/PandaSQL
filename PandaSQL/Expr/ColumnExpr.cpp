#include "stdafx.h"

#include "Expr/ColumnExpr.h"

#include "Expr/ExprContext.h"
#include "Expr/ExprWalker.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

ColumnExpr::ColumnExpr()
:
Expr(kExprColumnRef)
{
}

ColumnExpr::~ColumnExpr()
{
}

ColumnQualifiedName ColumnExpr::GetQualifiedColumnName() const
{
	return mQualifiedColumnName;
}

void ColumnExpr::SetQualifiedColumnName(const ColumnQualifiedName &inQualifiedColumnName)
{
	mQualifiedColumnName = inQualifiedColumnName;
}

bool ColumnExpr::IsTrue(const ExprContext &inExprContext) const
{
	bool result;

	Value columnValue;
		
	this->Eval(inExprContext, &columnValue);

	if (columnValue.GetType() == kUnknownType)
	{
		// It's a logic error if the current context doesn't include
		// the value for the operand, this means predicate(probably pushdown)
		// is not setup properly
		PDASSERT(0);

		result = false;
	}
	else
	{
		result = columnValue.GetAsBool();
	}

	return result;
}

Status ColumnExpr::Eval(const ExprContext &inExprContext, Value *io_value) const
{
	Status result;

	inExprContext.GetColumnValue(mQualifiedColumnName, io_value);

	return result;
}

Expr* ColumnExpr::CreateSubExprForPushdown(const std::vector<std::string> &inTableNameList) const
{
	Expr *result = NULL;

	std::vector<std::string>::const_iterator iter = inTableNameList.begin();
	for (; iter != inTableNameList.end(); iter++)
	{
		if (mQualifiedColumnName.tableName == *iter)
		{
			result = this->Clone();
			break;
		}
	}

	return result;
}

void ColumnExpr::Walk(ExprWalker *io_walker) const
{
	io_walker->Visit(this);
}

void ColumnExpr::MutableWalk(MutableExprWalker *io_walker)
{
	io_walker->MutableVisit(this);
}

Expr* ColumnExpr::Clone() const
{
	ColumnExpr *result = new ColumnExpr();

	result->mQualifiedColumnName = mQualifiedColumnName;

	return result;
}

void ColumnExpr::Print(uint32_t level) const
{
	this->PrintIndention(level);

	std::cout << mQualifiedColumnName << std::endl;
}

}	// PandaSQL