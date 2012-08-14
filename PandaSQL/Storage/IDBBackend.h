#ifndef PANDASQL_IDBBackend_H
#define PANDASQL_IDBBackend_H

#include "Utils/Types.h"
#include "Utils/Status.h"

namespace PandaSQL
{

class Table;

class IDBBackend
{
public:

	enum OpenMode
	{
		kNone				= 0,	
		kCreateIfMissing	= 0x00000001,
		kRead				= 0x00000002,
		kWrite				= 0x00000004,
	};

	static IDBBackend *CreateBackend(const std::string &inRootPath, StorageType inType);

	virtual ~IDBBackend() = 0 {}

	virtual Status CreateTable(const Table &inTable) = 0;
	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode) = 0;
	//virtual Status InsertRecord(const TupleData &inTuple) = 0;
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator) = 0;

protected:
	IDBBackend(const std::string &inRootPath);

	const std::string &mRootPath;

private:
	IDBBackend(const IDBBackend &rhs);
	IDBBackend& operator=(const IDBBackend &rhs);
};


}	// PandaSQL

#endif	// PANDASQL_IDBBackend_H