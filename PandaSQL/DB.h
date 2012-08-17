#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Catalog/Table.h"
#include "Catalog/TableCatalog.h"

#include "Utils/Join.h"
#include "Utils/Status.h"

#include <map>

namespace PandaSQL
{

class IVFS;
class File;
class TuplePredicate;
class IDBBackend;

class DB
{
public:

	struct Options
	{
		bool create_if_missing;

		Options();
	};

	static IVFS* CreateVFS();

	DB(StorageType inStorageType);
	~DB();
	Status Open(const std::string &inDBPath, const Options &inOptions);
	Status Close();
	Status CreateTable(const std::string &tableName, const Table::ColumnDefList &columnList);

	Status OpenTable(const std::string &tableName);

	Status InsertData(const std::string &tableName, const Table::ColumnDefList &columnList, const Table::ColumnValueList &columnValueList);
	Status DeleteData(const std::string &tableName, const TuplePredicate *inTuplePredicate = NULL);
	Status SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const Table::ColumnDefList &columnList, const TuplePredicate *inTuplePredicate = NULL);

	Table* GetTableByID(uint32_t inTableID) const;
	uint32_t GetTableIDByName(const std::string &inTableName) const;
	uint32_t GetColumnIDByName(const std::string &inColumnName) const;

private:

	typedef std::map<std::string, Table*> TableMap;
	typedef std::pair<std::string, Table*> TableMapEntry;

	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	Status GetTableByName_Private(const std::string &name, Table **o_table) const;

	StorageType mStorageType;
	IDBBackend *mpBackend;

	TableCatalog mTableCatalog;

	TableMap mTableMap;

	bool mIsOpen;
};

}	// PandaSQL

#endif	// PANDASQL_DB_H