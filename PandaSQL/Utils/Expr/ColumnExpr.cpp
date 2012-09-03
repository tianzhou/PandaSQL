#include "stdafx.h"

#include "ColumnExpr.h"

#include "Utils/Expr/ExprContext.h"
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

std::string ColumnExpr::GetColumnName() const
{
	return mQualifiedColumnName.columnName;
}

void ColumnExpr::SetColumnName(const std::string &inColumnName)
{
	mQualifiedColumnName.columnName = inColumnName;
}

std::string ColumnExpr::GetTableName() const
{
	return mQualifiedColumnName.tableName;
}

void ColumnExpr::SetTableName(const std::string &inTableName)
{
	mQualifiedColumnName.tableName = inTableName;
}

Status ColumnExpr::GetValue(ExprContext *io_exprContext, Value *io_value)
{
	Status result;

	io_exprContext->GetColumnValue(mQualifiedColumnName, io_value);

	return result;
}

}	// PandaSQL