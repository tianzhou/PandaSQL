#include "stdafx.h"

#include "DB.h"
#include "Storage/IDBBackend.h"

#include "Parser/ParserDriver.h"

#include "Catalog/Table.h"

#include "Storage/IStorage.h"

#include "Access/Iterator.h"
#include "Access/Tuple.h"

#include "VFS/WinVFS.h"

#include "Utils/Common.h"
#include "Utils/Debug.h"
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
	
		result = mpBackend->Open();

		mIsOpen = true;
	}

	return result;
}

Status DB::Close()
{
	PDASSERT(mIsOpen);

	Status result;

	result = mpBackend->Close();

	PDASSERT(result.OK());

	delete mpBackend;
	mpBackend = NULL;

	return result;
}

Status DB::CreateTable(const std::string &tableName, const Table::ColumnDefList &columnList)
{
	Status result;

	TableMap::iterator iter = mTableMap.find(tableName);

	PDASSERT(iter == mTableMap.end());

	if (iter == mTableMap.end())
	{
		Table *pTable = new Table();
		
		mTableMap.insert(TableMapEntry(tableName, pTable));

		result = mpBackend->CreateTable(tableName, columnList);
	}
	else
	{
		result = Status::kTableAlreadyExists;
	}

	return result;
}

Status DB::OpenTable(const std::string &tableName)
{
	Status result;

	result = mpBackend->OpenTable(tableName);
	
	return result;
}

Status DB::InsertData(const std::string &tableName, const Table::ColumnDefList &columnList, const Table::ColumnValueList &columnValueList)
{
	Status result;

	//Table *pTable = NULL;
	//result = this->GetTableByName_Private(tableName, &pTable);

	//if (result.OK())
	//{
	//	result = mpBackend->InsertData(tableName, columnList, columnValueList);
	//}

	TupleData oneTuple;

	Table::ColumnValueList::const_iterator iter = columnValueList.begin();

	std::string theValue;

	for (; iter != columnValueList.end(); iter++)
	{
		if (iter->type == kExprNumber)
		{
			NumberToString(iter->number, &theValue);
		}
		else if (iter->type == kExprText)
		{
			theValue = iter->text;
		}
		else if (iter->type == kExprColumnDef)
		{
			PDASSERT(0);
		}

		oneTuple.AppendData(theValue);
	}

	result = mpBackend->InsertData(tableName, NULL, oneTuple.ToString());

	return result;
}

Status DB::DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate /* = NULL */)
{
	Status result;

	Table *theTable = NULL;
	
	//result = DB::GetTableByName(tableName, &theTable);

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

		result = this->GetTableByName_Private(tableList[0], &theTable);

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

		result = this->GetTableByName_Private(joinList[0].tableName, &outerTable);

		if (result.OK())
		{
			result = this->GetTableByName_Private(joinList[1].tableName, &innerTable);

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

Status DB::GetTableByName_Private(const std::string &name, Table **o_table) const
{
	Status result;

	TableMap::const_iterator iter = mTableMap.find(name);

	if (iter != mTableMap.end())
	{
		*o_table = iter->second;
	}
	else
	{
		*o_table = NULL;

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
