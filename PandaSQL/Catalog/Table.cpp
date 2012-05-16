#include "stdafx.h"

#include "Table.h"

#include "VFS/IVFS.h"

#include "Storage/TupleImpl.h"
#include "Storage/Iterator.h"

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
}

Table::~Table()
{
	delete mpDataHost;
}

void Table::AddColumnDef(const ColumnDef &inColumDef)
{
	mColumnList.push_back(inColumDef);
}

Status Table::Open(IStorage::OpenMode openMode)
{
	return mpDataHost->OpenTable(this->GetName(), openMode);
}

Status Table::InsertRecord(const ColumnRefList &columnList, const ColumnValueList &columnValueList)
{
	Status result;

	TupleImpl oneTuple;

	ColumnValueList::const_iterator iter = columnValueList.begin();

	TupleImpl::FieldInfo oneField;

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
		else if (iter->type == kExprColumnRef)
		{
			PDASSERT(0);
		}

		oneTuple.AppendFieldData(theType, iter->text);
	}

	mpDataHost->InsertRecord(oneTuple);

	return result;
}

Status Table::SelectRecords(const ColumnRefList &columnList)
{
	Status result;

	//TODO
	Iterator *iter = NULL;
	result = mpDataHost->FindFirstRecordWithPredicate(NULL, &iter);

	if (result.OK())
	{
		std::string o_data;

		while (iter->Next().OK())
		{
			iter->GetValue(&o_data);
			std::cout << o_data << std::endl;
		}
		
	}

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