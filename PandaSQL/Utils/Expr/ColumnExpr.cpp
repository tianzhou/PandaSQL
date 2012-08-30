#include "stdafx.h"

#include "ColumnExpr.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

ColumnExpr::ColumnExpr()
{
}

ColumnExpr::~ColumnExpr()
{
}

std::string ColumnExpr::GetColumnName() const
{
	return mColumnName;
}

void ColumnExpr::SetColumnName(const std::string &inColumnName)
{
	mColumnName = inColumnName;
}

std::string ColumnExpr::GetTableName() const
{
	return mTableName;
}

void ColumnExpr::SetTableName(const std::string &inTableName)
{
	mTableName = inTableName;
}

}	// PandaSQL