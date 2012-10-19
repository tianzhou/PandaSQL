#include "stdafx.h"

#include "Storage/BerkeleyDB/BDBStorage.h"

#include "Access/Iterator.h"

namespace PandaSQL
{

static const uint32_t kCVSPageSize = 4096;

BDBStorage::BDBStorage(const std::string &inRootPath)
:
IStorage(inRootPath)
,mDataFileMode(IStorage::kNone)
,mpScanIterator(NULL)
{
}

BDBStorage::~BDBStorage()
{
	//this->ReleaseScanIterator(mpScanIterator);

	//delete mpHeapStore;
	//mpHeapStore = NULL;

	//delete mpPageProxy;
	//mpPageProxy = NULL;

	//mpVFS->CloseFile(mpDataFile);
	//mpDataFile = NULL;
}

Iterator* BDBStorage::CreateScanIterator(const TuplePredicate *inTuplePredicate /* = NULL */)
{
	Iterator *result = NULL;

	return result;
}

Iterator* BDBStorage::CreateIndexIterator()
{
	return NULL;
}

void BDBStorage::ReleaseScanIterator(Iterator *iter)
{
	delete iter;
}

void BDBStorage::ReleaseIndexIterator(Iterator *iter)
{
	delete iter;
}

Status BDBStorage::OpenTable(const std::string &inTableName, OpenMode inMode)
{
	Status result;

	//std::string tablePath = this->DataFilePathFromTableName(inTableName);

	//bool createIfMissing = inMode & kCreateIfMissing;

	//File *tableDataFile = NULL;

	//result = mpVFS->OpenFile(tablePath, createIfMissing, &tableDataFile);

	//if (result.OK())
	//{
	//	mpDataFile = tableDataFile;
	//	mDataFileMode = inMode;

	//	mpPageProxy = new PageProxy(kCVSPageSize, mpDataFile);

	//	//The data file is in heap format. Thus we use heap store to manipulate it
	//	mpHeapStore = new HeapStore(mpPageProxy);

	//	mpScanIterator = this->CreateScanIterator();
	//	PDASSERT(mpScanIterator);
	//}

	return result;
}

}	// PandaSQL