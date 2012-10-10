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

ColumnQualifiedName ColumnExpr::GetQualifiedColumnName() const
{
	return mQualifiedColumnName;
}

void ColumnExpr::SetQualifiedColumnName(const ColumnQualifiedName &inQualifiedColumnName)
{
	mQualifiedColumnName = inQualifiedColumnName;
}

bool ColumnExpr::IsTrue(ExprContext *io_exprContext) const
{
	return true;
}

Status ColumnExpr::GetValue(ExprContext *io_exprContext, Value *io_value) const
{
	Status result;

	io_exprContext->GetColumnValue(mQualifiedColumnName, io_value);

	return result;
}

void ColumnExpr::PopulateDependentColumns(TableAndColumnSetMap *io_tableAndColumnSetMap) const
{
	AddOneColumnToMap(mQualifiedColumnName, io_tableAndColumnSetMap);
}

}	// PandaSQL