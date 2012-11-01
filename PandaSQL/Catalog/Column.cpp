#include "stdafx.h"

#include "Catalog/Column.h"

#include "Utils/Debug.h"

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

void ProjectTuple(const ColumnDefList &inAllColDefList, const ColumnDefList &inProjectColDefList, const ValueList &inTupleValue, ValueList *o_projectTupleValue)
{
	ColumnDefList::const_iterator projectIter = inProjectColDefList.begin();
	ColumnDefList::const_iterator allColIter;

	o_projectTupleValue->clear();

	size_t index;
	for (; projectIter != inProjectColDefList.end(); projectIter++)
	{
		allColIter = inAllColDefList.begin();

		index = 0;
		for (; allColIter != inAllColDefList.end(); allColIter++)
		{
			if (projectIter->qualifiedName.tableName == allColIter->qualifiedName.tableName
				&& projectIter->qualifiedName.columnName == allColIter->qualifiedName.columnName)
			{
				o_projectTupleValue->push_back(inTupleValue[index]);
			}

			index++;
		}
	}
}

void ColumnDefListToTupleDesc(const ColumnDefList &colDefList, TupleDesc *io_tupleDesc)
{
	ColumnDefList::const_iterator iter = colDefList.begin();

	for (; iter != colDefList.end(); iter++)
	{
		TupleDescElement oneDescElement;

		oneDescElement.mDataType = iter->dataType;
		
		io_tupleDesc->push_back(oneDescElement);
	}
}

}	// PandaSQL
