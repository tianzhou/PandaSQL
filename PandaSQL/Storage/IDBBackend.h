#ifndef PANDASQL_IDBBackend_H
#define PANDASQL_IDBBackend_H

#include "Access/Tuple.h"


#include "Optimizer/Join/Join.h"

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class BooleanExpr;
class TupleIterator;

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

	typedef char* PayloadPtr;

	static IDBBackend *CreateBackend(const std::string &inRootPath, StorageType inType);

	virtual ~IDBBackend() = 0 {}

	virtual Status Open() = 0;
	virtual Status Close() = 0;

	virtual Status OpenTable(const std::string &tableName, OpenMode openMode, PayloadPtr *io_payload) = 0;
	virtual Status DropTable(const std::string &tableName, PayloadPtr payload) = 0;
	
	virtual Status OpenIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const UInt32List &tupleIndexList, bool isUnique, OpenMode openMode, PayloadPtr tablePayload, PayloadPtr *io_indexPayload) = 0;
	virtual Status DropIndex(const std::string &indexName, const std::string &tableName, PayloadPtr indexPayload) = 0;

	//Update/Delete/Select are through iterator
	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList, PayloadPtr tablePayload) = 0;

	//Return NULL if table for tableName is not opened
	virtual TupleIterator* CreateSeqScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const BooleanExpr *inTuplePredicate, PayloadPtr payload) = 0;
	virtual TupleIterator* CreateIndexScanIterator(const std::string &indexName, const UInt32List &tupleIndexList, const TupleDesc &tupleDesc, const BooleanExpr *inTuplePredicate, PayloadPtr payload) = 0;

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