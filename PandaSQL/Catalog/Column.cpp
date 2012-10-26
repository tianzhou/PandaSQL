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

ColumnDataFunctor::ColumnDataFunctor(const ColumnDefList &inColumnDefList, ValueList *io_valueList)
:
mColumnDefList(inColumnDefList)
,mpValueList(io_valueList)
{
}

ColumnDataFunctor::~ColumnDataFunctor()
{
}

void ColumnDataFunctor::operator()(const std::string &inTupleData)
{
	TupleStringToValueList(mColumnDefList, inTupleData, mpValueList);
}

void TupleElementToString(const TupleDescElement &descElement, const TupleDataElement &dataElement, std::string *o_string)
{
	if (descElement.mDataType == kInt)
	{
		o_string->append((const char *)&dataElement.mNumber, sizeof(dataElement.mNumber));
	}
	else if (descElement.mDataType == kText)
	{
		size_t length = dataElement.mText.length();
		o_string->append((const char *)&length, sizeof(size_t));
		o_string->append(dataElement.mText.c_str(), length);
	}
	else
	{
		PDASSERT(0);
	}
}

void TupleToString(const TupleDesc &desc, const TupleData &data, std::string *o_string)
{
	PDASSERT(desc.size() == data.size());

	o_string->clear();

	for (size_t i = 0; i < desc.size(); i++)
	{
		TupleElementToString(desc[i], data[i], o_string);
	}
}

void TupleStringToValue(const ColumnDef &inColumnDef, const std::string &inString, uint32_t *io_offset, Value *o_value)
{
	if (inColumnDef.dataType == kInt)
	{
		int numberValue = *(const int *)(inString.c_str() + *io_offset);
		o_value->SetAsNumber(numberValue);
		*io_offset += sizeof(int);
	}
	else if (inColumnDef.dataType == kText)
	{
		std::string stringValue;

		size_t length = *(const size_t *)(inString.c_str() + *io_offset);
		*io_offset += sizeof(length);

		stringValue.append(inString.c_str() + *io_offset, length);
		o_value->SetAsString(stringValue);
		
		*io_offset += length;
	}
	else
	{
		PDASSERT(0);
	}
}

void TupleStringToValueList(const ColumnDefList &inColumnDefList, const std::string &inString, ValueList *o_valueList)
{
	size_t offset = 0;

	o_valueList->clear();

	for (size_t i = 0; i < inColumnDefList.size(); i++)
	{
		Value oneValue;

		TupleStringToValue(inColumnDefList[i], inString, &offset, &oneValue);

		o_valueList->push_back(oneValue);
	}

	//TODO: Check
	PDASSERT(offset <= inString.length());
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
