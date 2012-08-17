#ifndef PANDASQL_IDBBackend_H
#define PANDASQL_IDBBackend_H

#include "Catalog/Table.h"

#include "Utils/Join.h"
#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class IDBBackend
{
public:

	enum
	{
		kNone				= 0,	
		kCreateIfMissing	= 0x00000001,
		kRead				= 0x00000002,
		kWrite				= 0x00000004,
	};

	typedef uint8_t OpenMode;

	static IDBBackend *CreateBackend(const std::string &inRootPath, StorageType inType);

	virtual ~IDBBackend() = 0 {}

	virtual Status Open() = 0;
	virtual Status Close() = 0;

	virtual Status CreateTable(const std::string &tableName, const Table::ColumnDefList &columnList) = 0;
	virtual Status OpenTable(const std::string &tableName) = 0;

	virtual Status InsertData(const std::string &tableName, const std::string *keyStr, const std::string &dataStr) = 0;
	virtual Status DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate = NULL) = 0;
	virtual Status SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const Table::ColumnDefList &columnList, const TuplePredicate *inTuplePredicate = NULL) = 0;
	//virtual Status InsertRecord(const TupleData &inTuple) = 0;
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator) = 0;

protected:
	IDBBackend(const std::string &inRootPath);

private:
	IDBBackend(const IDBBackend &rhs);
	IDBBackend& operator=(const IDBBackend &rhs);

protected:

	const std::string &mRootPath;

};


}	// PandaSQL

#endif	// PANDASQL_IDBBackend_H