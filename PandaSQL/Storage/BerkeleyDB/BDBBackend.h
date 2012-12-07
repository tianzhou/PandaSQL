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

	virtual Status OpenTable(const std::string &tableName, OpenMode openMode);
	virtual Status DropTable(const std::string &tableName);
	
	virtual Status OpenIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const std::vector<int32_t> &indexList, bool isUnique, OpenMode openMode);
	virtual Status DropIndex(const std::string &indexName, const std::string &tableName);

	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList);

	virtual TupleIterator* CreateScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const TuplePredicate *inTuplePredicate = NULL);

privileged:

	struct IndexInfo
	{
		DB *indexDB;
		TupleDesc tupleDesc;
		std::vector<int32_t> indexList;
	};

private:

	//Same structure in IndexCatalog
	struct IndexEntryKey
	{
		std::string indexName;
		std::string tableName;
		bool operator==(const IndexEntryKey &rhs) const;
		bool operator<(const IndexEntryKey &rhs) const;
	};

	BDBBackend(const BDBBackend &rhs);
	BDBBackend& operator=(const BDBBackend &rhs);

	Status GetTableByName_Private(const std::string &name, DB **o_table) const;
	Status GetIndexByName_Private(const std::string &indexName, const std::string &tableName, DB **o_index) const;

	typedef std::map<std::string, DB*> TableMap;
	typedef std::pair<std::string, DB*> TableMapEntry;

	typedef std::map<IndexEntryKey, IndexInfo> IndexMap;
	typedef std::pair<IndexEntryKey, IndexInfo> IndexMapEntry;

	DB_ENV *mpDBEnv;
	TableMap mTableMap;
	IndexMap mIndexMap;
	bool mIsOpen;
};


}	// PandaSQL

#endif	// PANDASQL_BDBBackend_H