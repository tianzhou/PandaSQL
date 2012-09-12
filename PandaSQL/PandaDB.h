#ifndef PANDASQL_PANDADB_H
#define PANDASQL_PANDADB_H

#include "Catalog/Column.h"
#include "Catalog/Table.h"
#include "Catalog/TableCatalog.h"

#include "Utils/Join.h"
#include "Utils/Status.h"

#include <map>

namespace PandaSQL
{

class BooleanExpr;
class File;
class IDBBackend;
class IVFS;
class TupleIterator;

class PandaDB
{
public:

	struct Options
	{
		bool create_if_missing;

		Options();
	};

	static IVFS* CreateVFS();

	PandaDB(StorageType inStorageType);
	~PandaDB();
	Status Open(const std::string &inDBPath, const Options &inOptions);
	Status Close();
	Status CreateTable(const std::string &tableName, const ColumnDefList &columnList);

	Status OpenTable(const std::string &tableName);

	Status InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList);
	Status DeleteData(const std::string &tableName, const BooleanExpr *inBooleanExpr = NULL);
	Status SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr = NULL);

	Status GetTableByName(const std::string &name, Table **o_table) const;

	Table* GetTableByID(uint32_t inTableID) const;
	uint32_t GetTableIDByName(const std::string &inTableName) const;
	uint32_t GetColumnIDByName(const std::string &inColumnName) const;

	Status GetColumnDefFromQualifiedName(const Table::TableRefList &inTableRefList, const ColumnQualifiedName &inQualifiedName, ColumnDef *io_columnDef) const;

	TupleIterator* CreateTupleIteratorForTable(const Table &inTable);
private:

	typedef std::map<std::string, Table*> TableMap;
	typedef std::pair<std::string, Table*> TableMapEntry;

	PandaDB(const PandaDB &rhs);
	PandaDB& operator=(const PandaDB &rhs);

	void	ClearTableMap_Private();

	StorageType mStorageType;
	IDBBackend *mpBackend;

	TableCatalog mTableCatalog;

	TableMap mTableMap;

	bool mIsOpen;
};

}	// PandaSQL

#endif	// PANDASQL_PANDADB_H