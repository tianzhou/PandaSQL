#include "stdafx.h"

#include "Column.h"


namespace PandaSQL
{

ColumnQualifiedName::ColumnQualifiedName()
{
}

ColumnQualifiedName::ColumnQualifiedName(const std::string inTableName, const std::string inColumnName)
:
tableName(inTableName)
,columnName(inColumnName)
{
}

bool ColumnQualifiedName::operator<(const ColumnQualifiedName &rhs) const
{
	if (this->tableName == rhs.tableName)
	{
		return this->columnName < rhs.columnName;
	}

	return this->tableName < rhs.tableName;
}

ColumnDef::ColumnDef()
:
qualifiedName()
,index(kInvalidColumnIndex)
,dataType(kUnknownType)
,constraintType(kConstraintNone)
{
	
}

}	// PandaSQL
