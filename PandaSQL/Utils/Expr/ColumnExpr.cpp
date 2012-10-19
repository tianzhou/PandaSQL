#include "stdafx.h"

#include "ColumnExpr.h"

#include "Utils/Expr/ExprContext.h"
#include "Utils/Expr/ExprWalker.h"
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
		
	this->GetValue(inExprContext, &columnValue);

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

Status ColumnExpr::GetValue(const ExprContext &inExprContext, Value *io_value) const
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

Expr* ColumnExpr::Clone() const
{
	ColumnExpr *result = new ColumnExpr();

	result->mQualifiedColumnName = mQualifiedColumnName;

	return result;
}

}	// PandaSQL