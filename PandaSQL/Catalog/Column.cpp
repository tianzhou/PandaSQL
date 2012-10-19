#include "stdafx.h"

#include "Catalog/Column.h"

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

bool ColumnQualifiedName::operator==(const ColumnQualifiedName &rhs) const
{
	return this->tableName == rhs.tableName
		&& this->columnName == rhs.columnName
		;
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

bool ColumnDef::operator==(const ColumnDef &rhs) const
{
	return this->qualifiedName == rhs.qualifiedName
		&& this->index == rhs.index
		&& this->dataType == rhs.dataType
		&& this->constraintType == rhs.constraintType
		;
}

void AddOneColumnToMap(const ColumnQualifiedName &inColumnQualifiedName, TableAndColumnSetMap *io_tableAndColumnSetMap)
{
	TableAndColumnSetMap::iterator iter = io_tableAndColumnSetMap->find(inColumnQualifiedName.tableName);

	if (iter == io_tableAndColumnSetMap->end())
	{
		ColumnNameSet newColumnSet;
		newColumnSet.insert(inColumnQualifiedName.columnName);
		(*io_tableAndColumnSetMap)[inColumnQualifiedName.tableName] = newColumnSet;
	}
	else
	{
		iter->second.insert(inColumnQualifiedName.columnName);
	}
}

}	// PandaSQL
