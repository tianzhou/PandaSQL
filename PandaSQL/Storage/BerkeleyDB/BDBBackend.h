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

	virtual Status OpenTable(const std::string &tableName, OpenMode openMode, PayloadPtr *io_payload);
	virtual Status DropTable(const std::string &tableName, PayloadPtr payload);
	
	virtual Status OpenIndex(const std::string &indexName, const std::string &tableName, const TupleDesc &tupleDesc, const std::vector<int32_t> &indexList, bool isUnique, OpenMode openMode, PayloadPtr tablePayload, PayloadPtr *io_indexPayload);
	virtual Status DropIndex(const std::string &indexName, const std::string &tableName, PayloadPtr indexPayload);

	virtual Status InsertData(const std::string &tableName, const TupleDesc &tupleDesc, const ValueList &tupleValueList, PayloadPtr payload);

	virtual TupleIterator* CreateScanIterator(const std::string &tableName, const TupleDesc &tupleDesc, const TuplePredicate *inTuplePredicate, PayloadPtr payload);

privileged:

	struct IndexInfo
	{
		TupleDesc tupleDesc;
		std::vector<int32_t> columnIndexList;
	};

private:

	BDBBackend(const BDBBackend &rhs);
	BDBBackend& operator=(const BDBBackend &rhs);

	typedef std::map<DB*, IndexInfo> IndexMap;
	typedef std::pair<DB*, IndexInfo> IndexMapEntry;

	DB_ENV *mpDBEnv;
	bool mIsOpen;

	IndexMap mIndexMap;
};


}	// PandaSQL

#endif	// PANDASQL_BDBBackend_H