#include "stdafx.h"

#include "CVSStorage.h"
#include "CVSScanIterator.h"
#include "HeapStore.h"
#include "Access/Tuple.h"
#include "Storage/Buffer/PageProxy.h"

#include "VFS/IVFS.h"
#include "VFS/File.h"

namespace PandaSQL
{

static const uint32_t kCVSPageSize = 4096;

CVSStorage::CVSStorage(IVFS *io_VFS, const std::string &inRootPath)
:IStorage(inRootPath, io_VFS)
,mpDataFile(NULL)
,mDataFileMode(IStorage::kNone)
,mpHeapStore(NULL)
,mpPageProxy(NULL)
,mpScanIterator(NULL)
{
}

CVSStorage::~CVSStorage()
{
	this->ReleaseScanIterator(mpScanIterator);

	delete mpHeapStore;
	mpHeapStore = NULL;

	delete mpPageProxy;
	mpPageProxy = NULL;

	mpVFS->CloseFile(mpDataFile);
	mpDataFile = NULL;
}

Iterator* CVSStorage::CreateScanIterator(const Predicate *inPredicate /* = NULL */)
{
	Iterator *result = NULL;

	if (mpPageProxy)
	{
		result = new CVSScanIterator(inPredicate, mpPageProxy);
	}

	return result;
}

Iterator* CVSStorage::CreateIndexIterator()
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

		mpPageProxy = new PageProxy(kCVSPageSize, mpDataFile);

		//The data file is in heap format. Thus we use heap store to manipulate it
		mpHeapStore = new HeapStore(mpPageProxy);

		mpScanIterator = this->CreateScanIterator();
		PDASSERT(mpScanIterator);
	}

	return result;
}

//Status CVSStorage::InsertRecord(const Tuple &inTuple)
//{
//	PDASSERT(mpHeapStore);
//
//	Status result;
//
//	uint32_t fieldCount = inTuple.Count();
//	
//	std::string rowData;
//	std::string o_data;
//	for (uint32_t i = 0; i < fieldCount; i++)
//	{
//		if (i != 0)
//		{
//			rowData += ",";
//		}
//
//		inTuple.GetDataOfField(i, &o_data);
//		rowData += o_data;
//	}
//
//	rowData += kNewLineSymbol;
//
//	PDASSERT(rowData.length() <= kCVSPageSize);
//
//	uint32_t pageCount;
//	uint32_t pageNum;
//	result = mpPageProxy->GetPageCount(&pageCount);
//
//	if (result.OK())
//	{
//		char *pageData = NULL;
//		if (pageCount == 0)
//		{
//			result = mpPageProxy->NewPage(&pageNum);
//		}
//		else
//		{
//			//Get last page, page num is 0 based
//			pageNum = pageCount - 1;
//		}
//
//		if (result.OK())
//		{
//			result = mpPageProxy->GetPage(pageNum
//				, PageProxy::kPageRead | PageProxy::kPageWrite
//				, &pageData);
//
//			char *pch =strchr(pageData, '\0');
//			
//			if (pch)
//			{
//				//TODO: Hard code
//				if ((File::Size)(pch - pageData) <= kCVSPageSize - rowData.length())
//				{
//					memcpy(pch, rowData.c_str(), rowData.length());
//				}
//				else
//				{
//					//TODO: Need concatenate between two pages.
//					//No enough space, add a new page
//					result = mpPageProxy->NewPage(&pageNum);
//
//					if (result.OK())
//					{
//						result = mpPageProxy->GetPage(pageNum
//							, PageProxy::kPageRead | PageProxy::kPageWrite
//							, &pageData);
//
//						if (result.OK())
//						{
//							memcpy(pageData, rowData.c_str(), rowData.length());
//						}
//					}
//				}
//			}
//
//			if (result.OK())
//			{
//				mpPageProxy->PutPage(pageNum, true, pageData);
//			}
//		}
//	}
//
//	return result;
//}

//Status CVSStorage::FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator)
//{
//	Status result;
//
//	*o_iterator = this->CreateScanIterator();
//
//	return result;
//}

//private
std::string CVSStorage::DataFilePathFromTableName(const std::string &tableName)
{
	return mRootPath + "\\" + tableName + ".csv";
}

}	// PandaSQL