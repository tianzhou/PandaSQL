#ifndef PANDASQL_ISTORAGE_H
#define PANDASQL_ISTORAGE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class VFS;
class ITuple;

/*
	Each table contains an IStorage pointer as its data host.
	Different table can have different host.
*/

class IStorage
{
public:

	enum StorageType
	{
		kCVS
	};

	enum OpenMode
	{
		kNone				= 0,	
		kCreateIfMissing	= 0x00000001,
		kRead				= 0x00000002,
		kWrite				= 0x00000004,
	};

	static IStorage *CreateStorage(const std::string &inDBRootPath, StorageType inType, VFS *io_VFS);

	virtual ~IStorage() = 0 {}

	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode) = 0;
	virtual Status InsertRow(const ITuple &inTuple) = 0;

protected:
	IStorage(const std::string &inRootPath, VFS *io_VFS);

	const std::string &mRootPath;
	VFS *mpVFS;

private:
	IStorage(const IStorage &rhs);
	IStorage& operator=(const IStorage &rhs);
};

class IStorageHelper
{
public:

	virtual ~IStorageHelper() = 0 {}

protected:
	IStorageHelper();

private:
	IStorageHelper(const IStorageHelper &rhs);
	IStorageHelper& operator=(const IStorageHelper &rhs);

};

}	// PandaSQL

#endif	// PANDASQL_ISTORAGE_H