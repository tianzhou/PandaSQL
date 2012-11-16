#ifndef PANDASQL_BDBBackend_H
#define PANDASQL_BDBBackend_H

#include <map>
#include <string>

#include <db_cxx.h>

#include "Storage/IDBBackend.h"

namespace PandaSQL
{

class BDBBackend : public IDBBackend
{
public:

	BDBBackend(const std::string &inRootPath);
	virtual ~BDBBackend();

	virtual Status Open();
	virtual Status Close();

	virtual Status OpenTable(const std::string &inTableName, OpenMode openMode);
	virtual Status CloseTable(const std::string &inTableName);
	virtual Status DropTable(const std::string &tableName);
	
	virtual Status CreateIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const std::vector<int32_t> &indexList, bool isUnique);
	//virtual Status CreateIndex(
	//virtual Status InsertRecord(const TupleData &inTuple) = 0;
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator) = 0;

	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList, int32_t keyIndex);
	virtual Status DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate = NULL);
	virtual Status SelectData(const std::string &tableName, const ColumnDefList &columnList, const TuplePredicate *inTuplePredicate = NULL);

	virtual TupleIterator* CreateScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const TuplePredicate *inTuplePredicate = NULL);

privileged:

	struct IndexInfo
	{
		TupleDesc tupleDesc;
		std::vector<int32_t> indexList;
	};

private:

	BDBBackend(const BDBBackend &rhs);
	BDBBackend& operator=(const BDBBackend &rhs);

	Status GetTableByName_Private(const std::string &name, DB **o_table) const;

	typedef std::map<std::string, DB*> TableMap;
	typedef std::pair<std::string, DB*> TableMapEntry;

	typedef std::map<DB*, IndexInfo> IndexMap;
	typedef std::pair<DB*, IndexInfo> IndexMapEntry;

	DB_ENV *mpDBEnv;
	TableMap mTableMap;
	IndexMap mIndexMap;
	bool mIsOpen;
};


}	// PandaSQL

#endif	// PANDASQL_BDBBackend_H