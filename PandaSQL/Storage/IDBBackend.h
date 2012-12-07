#ifndef PANDASQL_IDBBackend_H
#define PANDASQL_IDBBackend_H

#include "Access/Tuple.h"

#include "Optimizer/Join/Join.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class TupleIterator;
class TuplePredicate;

class IDBBackend
{
public:

	enum
	{
		kNone				= 0,	
		kCreate				= 0x00000001,
		kErrorIfExists		= 0x00000002,
		kRead				= 0x00000004,
		kWrite				= 0x00000008,
	};

	typedef uint8_t OpenMode;

	static IDBBackend *CreateBackend(const std::string &inRootPath, StorageType inType);

	virtual ~IDBBackend() = 0 {}

	virtual Status Open() = 0;
	virtual Status Close() = 0;

	virtual Status OpenTable(const std::string &tableName, OpenMode openMode) = 0;
	virtual Status DropTable(const std::string &tableName) = 0;
	
	virtual Status OpenIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const std::vector<int32_t> &indexList, bool isUnique, OpenMode openMode) = 0;
	virtual Status DropIndex(const std::string &indexName, const std::string &tableName) = 0;

	//Update/Delete/Select are through iterator
	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList) = 0;

	//Return NULL if table for tableName is not opened
	virtual TupleIterator* CreateScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const TuplePredicate *inTuplePredicate = NULL) = 0;

protected:
	IDBBackend(const std::string &inRootPath);

private:
	IDBBackend(const IDBBackend &rhs);
	IDBBackend& operator=(const IDBBackend &rhs);

protected:

	const std::string mRootPath;

};


}	// PandaSQL

#endif	// PANDASQL_IDBBackend_H