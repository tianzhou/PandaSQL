#include "stdafx.h"

#include "DB.h"
#include "Storage/IDBBackend.h"

#include "Parser/ParserDriver.h"

#include "Catalog/Table.h"

#include "Storage/IStorage.h"

#include "Access/Iterator.h"
#include "Access/Tuple.h"

#include "VFS/WinVFS.h"

#include "Utils/Predicate.h"

#include <iostream>

namespace PandaSQL
{

//static
IVFS* DB::CreateVFS()
{
	//Only have windows for now
	return new WinVFS();
}

DB::Options::Options()
:
create_if_missing(false)
{
}

DB::DB(StorageType inStorageType)
:
mStorageType(inStorageType)
,mpBackend(NULL)
,mIsOpen(false)
{
}

DB::~DB()
{
	PDASSERT(!mpBackend);
}

Status DB::Open(const std::string &inDBPath, const Options &inOptions)
{
	Status result;

	if (mIsOpen)
	{
		result = Status::kInvalidCommand;
	}
	else
	{
		PDASSERT(!mpBackend);

		mpBackend = IDBBackend::CreateBackend(inDBPath, mStorageType);
	
		mIsOpen = true;
	}

	return result;
}

Status DB::Close()
{
	PDASSERT(mIsOpen);

	Status result;

	delete mpBackend;
	mpBackend = NULL;

	TableList::iterator iter = mTableList.begin();

	for (; iter != mTableList.end(); iter++)
	{
		delete *iter;
	}

	return result;
}

Status DB::CreateTable(const Table &inTable)
{
	Status result;

	return result;
}

Status DB::LoadTable(Table *io_pTable)
{
	Status result;

	IStorage::OpenMode mode = IStorage::OpenMode(IStorage::kCreateIfMissing | IStorage::kRead | IStorage::kWrite);
	result = io_pTable->Open(mode);

	if (result.OK())
	{
		mTableList.push_back(io_pTable);
	}
	
	return result;
}

Status DB::InsertData(const std::string &tableName, const Table::ColumnDefList &columnList, const Table::ColumnValueList &columnValueList)
{
	Status result;

	Table *theTable = NULL;
	
	result = DB::GetTableByName(tableName, &theTable);

	if (result.OK())
	{
		result = theTable->AddRecord(columnList, columnValueList);
	}

	return result;
}

Status DB::DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate /* = NULL */)
{
	Status result;

	Table *theTable = NULL;
	
	result = DB::GetTableByName(tableName, &theTable);

	if (result.OK())
	{
		result = theTable->DeleteRecord(inTuplePredicate);
	}

	return result;
}

Status DB::SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const Table::ColumnDefList &columnList, const TuplePredicate *inTuplePredicate /*= NULL*/)
{
	Status result;

	if (tableList.size() == 1)
	{
		Table *theTable = NULL;

		result = DB::GetTableByName(tableList[0], &theTable);

		if (result.OK())
		{
			std::cout << "****** Select Table:" << tableList[0] << " ******" << std::endl;
			result = theTable->SelectRecords(columnList, inTuplePredicate);
		}
	}
	else if (tableList.size() == 2)
	{
		Table *outerTable = NULL;
		Table *innerTable = NULL;

		result = DB::GetTableByName(joinList[0].tableName, &outerTable);

		if (result.OK())
		{
			result = DB::GetTableByName(joinList[1].tableName, &innerTable);

			if (result.OK())
			{
				Iterator *outerScan = outerTable->CreateScanIterator();

				PDASSERT(outerScan);

				while (outerScan->Valid())
				{
					TupleData outerTuple;

					result = outerScan->GetValue(&outerTuple);

					if (!result.OK())
					{
						break;
					}

					std::vector<TupleEntry> tupleContext;
					TupleEntry outerTupleEntry = {outerTable->GetName(), outerTuple};
					tupleContext.push_back(outerTupleEntry);

					Iterator *innerScan = innerTable->CreateScanIterator();

					PDASSERT(innerScan);

					while (innerScan->Valid())
					{
						TupleData innerTuple;

						result = innerScan->GetValue(&innerTuple);

						if (!result.OK())
						{
							break;
						}

						//if (!inTuplePredicate
						//	|| inTuplePredicate->Eval(tupleContext))
						//{
						//	std::cout << outerTuple.ToString() << std::endl;
						//}

						innerScan->Next();
					}

					delete innerScan;

					if (!result.OK())
					{
						break;
					}

					outerScan->Next();
				}

				delete outerScan;
			}
		}
	}
	else
	{
		PDASSERT(0);
	}

	return result;
}

//Private
Status DB::GetTableByName(const std::string &name, Table **o_table) const
{
	Status result;

	*o_table = NULL;

	TableList::const_iterator iter = mTableList.begin();

	for(; iter != mTableList.end(); iter++)
	{
		if (name == (*iter)->GetName())
		{
			*o_table = *iter;
			break;
		}
	}

	if (iter == mTableList.end())
	{
		result = Status::kTableMissing;
	}

	return result;
}

Table* DB::GetTableByID(uint32_t inTableID) const
{
	Table *result = NULL;

	return result;
}

uint32_t DB::GetTableIDByName(const std::string &inTableName) const
{
	uint32_t result = kUnknownID;

	return result;
}

uint32_t DB::GetColumnIDByName(const std::string &inColumnName) const
{
	uint32_t result = kUnknownID;

	return result;
}


}	// PandaSQL
