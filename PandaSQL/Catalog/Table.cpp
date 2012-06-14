#include "stdafx.h"

#include "Table.h"

#include "VFS/IVFS.h"

#include "Storage/Tuple.h"
#include "Storage/Iterator.h"

#include "Utils/Predicate.h"

#include <iostream>

namespace PandaSQL
{

Table::Table(const std::string &inDBRootPath, IStorage::StorageType inType, IVFS *io_VFS)
:
mpVFS(io_VFS)
,mpDataHost(NULL)
,mpScanIterator(NULL)
{
	mpDataHost = IStorage::CreateStorage(inDBRootPath, inType, mpVFS);
	//mpScanIterator = mpDataHost->CreateScanIterator();
}

Table::~Table()
{
	delete mpScanIterator;
	delete mpDataHost;
}

void Table::AddColumnDef(const ColumnDef &inColumDef)
{
	mColumnList.push_back(inColumDef);
}

Status Table::GetColumnByName(const std::string &name, ColumnDef *o_columnDef) const
{
	Status result;

	ColumnDefList::const_iterator iter = mColumnList.begin();

	for(; iter != mColumnList.end(); iter++)
	{
		if (name == iter->qualifiedName.columnName)
		{
			*o_columnDef = *iter;
			break;
		}
	}

	if (iter == mColumnList.end())
	{
		result = Status::kColumnMissing;
	}

	return result;
}

Status Table::Open(IStorage::OpenMode openMode)
{
	return mpDataHost->OpenTable(this->GetName(), openMode);
}

Status Table::AddRecord(const ColumnRefList &columnList, const ColumnValueList &columnValueList)
{
	Status result;

	Tuple oneTuple;

	ColumnValueList::const_iterator iter = columnValueList.begin();

	Tuple::FieldInfo oneField;

	DataType theType;

	for (; iter != columnValueList.end(); iter++)
	{
		if (iter->type == kExprNumber)
		{
			theType = kInt;
		}
		else if (iter->type == kExprText)
		{
			theType = kText;
		}
		else if (iter->type == kExprColumnDef)
		{
			PDASSERT(0);
		}

		oneTuple.AppendFieldData(theType, iter->text);
	}

	Iterator *theIter = mpDataHost->CreateScanIterator();

	result = theIter->SeekAfterLast();

	if (result.OK())
	{
		result = theIter->InsertValue(oneTuple);
	}

	delete theIter;

	return result;
}

Status Table::DeleteRecord(const Predicate *inPredicate /* = NULL */)
{
	Status result;

	Iterator *theIter = mpDataHost->CreateScanIterator(inPredicate);

	uint32_t i = 0;

	while (theIter->Valid())
	{
		result = theIter->DeleteValue();

		if (!result.OK())
		{
			break;
		}

		theIter->Next();

		i++;
	}

	delete theIter;

	return result;
}

Status Table::SelectRecords(const ColumnRefList &columnList)
{
	Status result;

	Iterator *theIter = mpDataHost->CreateScanIterator();

	while (theIter->Valid())
	{
		Tuple theTuple;

		result = theIter->GetValue(&theTuple);

		if (!result.OK())
		{
			break;
		}

		std::cout << theTuple.ToString() << std::endl;

		theIter->Next();
	}

	delete theIter;

	return result;
}

//private
Iterator* Table::GetScanIterator()
{
	if (!mpScanIterator
		&& mpDataHost)
	{
		mpScanIterator = mpDataHost->CreateScanIterator();
	}

	PDASSERT(mpScanIterator);

	return mpScanIterator;
}

}	// PandaSQL