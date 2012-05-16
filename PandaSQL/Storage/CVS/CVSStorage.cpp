#include "stdafx.h"

#include "CVSStorage.h"
#include "CVSScanIterator.h"
#include "HeapStore.h"
#include "Storage/ITuple.h"

#include "VFS/IVFS.h"
#include "VFS/File.h"

namespace PandaSQL
{

CVSStorage::CVSStorage(IVFS *io_VFS, const std::string &inRootPath)
:IStorage(inRootPath, io_VFS)
,mpDataFile(NULL)
,mDataFileMode(IStorage::kNone)
,mpHeapStore(NULL)
,mpScanIterator(NULL)
{
}

CVSStorage::~CVSStorage()
{
	this->ReleaseScanIterator(mpScanIterator);
	delete mpHeapStore;
	mpVFS->CloseFile(mpDataFile);
}

Iterator *CVSStorage::CreateScanIterator()
{
	Iterator *result = NULL;

	if (mpDataFile)
	{
		result = new CVSScanIterator(mpDataFile);
	}

	return result;
}

Iterator *CVSStorage::CreateIndexIterator()
{
	return NULL;
}

void CVSStorage::ReleaseScanIterator(Iterator *iter)
{
	delete iter;
}

void CVSStorage::ReleaseIndexIterator(Iterator *iter)
{
	delete iter;
}

Status CVSStorage::OpenTable(const std::string &inTableName, OpenMode inMode)
{
	Status result;

	std::string tablePath = this->DataFilePathFromTableName(inTableName);

	bool createIfMissing = inMode & kCreateIfMissing;

	File *tableDataFile = NULL;

	result = mpVFS->OpenFile(tablePath, createIfMissing, &tableDataFile);

	if (result.OK())
	{
		mpDataFile = tableDataFile;
		mDataFileMode = inMode;

		//The data file is in heap format. Thus we use heap store to manipulate it
		mpHeapStore = new HeapStore(tableDataFile);

		mpScanIterator = this->CreateScanIterator();
		PDASSERT(mpScanIterator);
	}

	return result;
}

Status CVSStorage::InsertRecord(const ITuple &inTuple)
{
	PDASSERT(mpDataFile);

	Status result;

	uint32_t fieldCount = inTuple.Count();
	
	std::string rowData;
	std::string o_data;
	for (uint32_t i = 0; i < fieldCount; i++)
	{
		if (i != 0)
		{
			rowData += ",";
		}

		inTuple.GetDataOfField(i, &o_data);
		rowData += o_data;
	}

	rowData += kNewLineSymbol;

	result = mpHeapStore->InsertRecord(rowData.length(), rowData.c_str());

	return result;
}

Status CVSStorage::FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator)
{
	Status result;

	*o_iterator = this->CreateScanIterator();

	return result;
}

//private
std::string CVSStorage::DataFilePathFromTableName(const std::string &tableName)
{
	return mRootPath + "\\" + tableName + ".csv";
}

}	// PandaSQL