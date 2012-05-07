#include "stdafx.h"

#include "CVSStorage.h"
#include "Storage/ITuple.h"

#include "VFS/IVFS.h"
#include "VFS/File.h"

namespace PandaSQL
{

CVSStorage::CVSStorage(IVFS *io_VFS, const std::string &inRootPath)
:IStorage(inRootPath, io_VFS)
,mpDataFile(NULL)
,mDataFileMode(IStorage::kNone)
{
}

CVSStorage::~CVSStorage()
{
	mpVFS->CloseFile(mpDataFile);
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
	}

	return result;
}

Status CVSStorage::InsertRow(const ITuple &inTuple)
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

	result = mpDataFile->WriteAppend(rowData.length(), rowData.c_str(), NULL);

	return result;
}

//private
std::string CVSStorage::DataFilePathFromTableName(const std::string &tableName)
{
	return mRootPath + "\\" + tableName + ".csv";
}

}	// PandaSQL