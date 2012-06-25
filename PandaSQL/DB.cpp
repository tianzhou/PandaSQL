#include "stdafx.h"

#include "DB.h"
#include "VFS/IVFS.h"
#include "VFS/WinVFS.h"
#include "VFS/File.h"

#include "Parser/ParserDriver.h"

#include "Catalog/Table.h"

#include "Storage/IStorage.h"
#include "Storage/Iterator.h"
#include "Storage/Tuple.h"

#include "Utils/Predicate.h"

#include <iostream>

namespace PandaSQL
{

static IVFS* create_vfs()
{
	//Only have windows for now
	return new WinVFS();
}

DB::Options::Options()
:
create_if_missing(false)
{
}

DB::DB()
:
mpVFS(NULL)
,mpMainFile(NULL)
,mpTableFile(NULL)
{
}

DB::~DB()
{
	PDASSERT(!mpVFS);
	PDASSERT(!mpMainFile);
	PDASSERT(!mpTableFile);
}

Status DB::Open(const std::string &inDBPath, const Options &inOptions)
{
	Status result;

	mDBPath = inDBPath;

	if (!mpVFS)
	{
		mpVFS = create_vfs();
	}

	if (!mpVFS->IsFileExist(inDBPath))
	{
		result = mpVFS->CreateDir(inDBPath);	
	}

	bool create_if_missing = inOptions.create_if_missing;

	if (result.OK())
	{
		std::string mainFilePath = inDBPath + "\\testDB.pdm";
		result = mpVFS->OpenFile(mainFilePath, create_if_missing, &mpMainFile);

		if (result.OK())
		{
			std::string tableFilePath = inDBPath + "\\testDB.pdt";
			result = mpVFS->OpenFile(tableFilePath, create_if_missing, &mpTableFile);

			if (result.OK())
			{
				ParserDriver parser(this);
				parser.SetLoadTable(true);
				result = parser.LoadFromFile(mpTableFile);
			}
		}
	}

	if (!result.OK())
	{
		this->Close();
	}

	return result;
}

Status DB::Close()
{
	Status result;

	TableList::iterator iter = mTableList.begin();

	for (; iter != mTableList.end(); iter++)
	{
		delete *iter;
	}

	mpVFS->CloseFile(mpMainFile);
	mpMainFile = NULL;
	mpVFS->CloseFile(mpTableFile);
	mpTableFile = NULL;

	delete mpVFS;
	mpVFS = NULL;

	return result;
}

Status DB::CreateTable(const std::string &inCreateStmt)
{
	Status result;

	std::string theString = inCreateStmt + '\n';
	result = mpTableFile->Append(theString.length(), theString.c_str(), NULL);
	
	if (result.OK())
	{
		result = mpTableFile->Flush();
	}

	return result;
}

Status DB::LoadTable(Table *pTable)
{
	Status result;

	IStorage::OpenMode mode = IStorage::OpenMode(IStorage::kCreateIfMissing | IStorage::kRead | IStorage::kWrite);
	result = pTable->Open(mode);

	if (result.OK())
	{
		mTableList.push_back(pTable);
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

Status DB::DeleteData(const std::string &tableName, const Predicate *inPredicate /* = NULL */)
{
	Status result;

	Table *theTable = NULL;
	
	result = DB::GetTableByName(tableName, &theTable);

	if (result.OK())
	{
		result = theTable->DeleteRecord(inPredicate);
	}

	return result;
}

Status DB::SelectData(const Table::TableRefList &tableList, const Table::ColumnDefList &columnList, const Predicate *inPredicate /*= NULL*/)
{
	Status result;

	if (tableList.size() == 1)
	{
		Table *theTable = NULL;

		result = DB::GetTableByName(tableList[0], &theTable);

		if (result.OK())
		{
			std::cout << "****** Select Table:" << tableList[0] << " ******" << std::endl;
			result = theTable->SelectRecords(columnList, inPredicate);
		}
	}
	else if (tableList.size() == 2)
	{
		Table *outerTable = NULL;
		Table *innerTable = NULL;

		result = DB::GetTableByName(tableList[0], &outerTable);

		if (result.OK())
		{
			result = DB::GetTableByName(tableList[1], &innerTable);

			if (result.OK())
			{
				Iterator *outerScan = outerTable->CreateScanIterator();

				PDASSERT(outerScan);

				while (outerScan->Valid())
				{
					Tuple outerTuple;

					result = outerScan->GetValue(&outerTuple);

					if (!result.OK())
					{
						break;
					}

					Iterator *innerScan = innerTable->CreateScanIterator();

					PDASSERT(innerScan);

					while (innerScan->Valid())
					{
						Tuple innerTuple;

						result = innerScan->GetValue(&innerTuple);

						if (!result.OK())
						{
							break;
						}

						if (!inPredicate
							|| inPredicate->Eval(&outerTuple, &innerTuple))
						{
							std::cout << outerTuple.ToString() << std::endl;
						}

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

//uint32_t DB::GetColumnIDByName(const std::string &inColumnName) const
//{
//	uint32_t result = kUnknownID;
//
//	return result;
//}


}	// PandaSQL
