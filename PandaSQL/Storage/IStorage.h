#ifndef PANDASQL_ISTORAGE_H
#define PANDASQL_ISTORAGE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class TuplePredicate;
class Iterator;

/*
	Each table contains an IStorage pointer as its data host.
	Different table can have different host.
*/

class IStorage
{
public:

	enum OpenMode
	{
		kNone				= 0,	
		kCreateIfMissing	= 0x00000001,
		kRead				= 0x00000002,
		kWrite				= 0x00000004,
	};

	static IStorage *CreateStorage(const std::string &inDBRootPath, StorageType inType);

	virtual ~IStorage() = 0 {}

	virtual Iterator *CreateScanIterator(const TuplePredicate *inTuplePredicate = NULL) = 0;
	virtual Iterator *CreateIndexIterator() = 0; 

	virtual void ReleaseScanIterator(Iterator *iter) = 0;
	virtual void ReleaseIndexIterator(Iterator *iter) = 0; 

	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode) = 0;
	//virtual Status InsertRecord(const TupleData &inTuple) = 0;
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator) = 0;

protected:
	IStorage(const std::string &inRootPath);

	const std::string &mRootPath;

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