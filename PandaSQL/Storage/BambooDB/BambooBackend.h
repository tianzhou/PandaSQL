#ifndef PANDASQL_BAMBOO_H
#define PANDASQL_BAMBOO_H

#include <string>

#include "Storage/IDBBackend.h"

class DBHandle;

namespace PandaSQL
{

class BambooBackend : public IDBBackend
{
public:

	BambooBackend(const std::string &inRootPath);
	virtual ~BambooBackend();

	virtual Status Open();
	virtual Status Close();

	virtual Status OpenTable(const std::string &tableName, OpenMode openMode, PayloadPtr *io_payload);
	virtual Status DropTable(const std::string &tableName, PayloadPtr payload);
	
	virtual Status OpenIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const UInt32List &tupleIndexList, bool isUnique, OpenMode openMode, PayloadPtr tablePayload, PayloadPtr *io_indexPayload);
	virtual Status DropIndex(const std::string &indexName, const std::string &tableName, PayloadPtr indexPayload);

	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList, PayloadPtr payload);

	virtual TupleIterator* CreateSeqScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const Expr *inTuplePredicate, PayloadPtr payload);
	virtual TupleIterator* CreateIndexScanIterator(const std::string &indexName, const UInt32List &tupleIndexList, const TupleDesc &tupleDesc, const Expr *inTuplePredicate, PayloadPtr payload);

privileged:


private:

	BambooBackend(const BambooBackend &rhs);
	BambooBackend& operator=(const BambooBackend &rhs);

	DBHandle *mpDBHandle;
	bool mIsOpen;
};


}	// PandaSQL

#endif	// PANDASQL_BAMBOO_H