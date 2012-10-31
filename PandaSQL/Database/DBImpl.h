#ifndef PANDASQL_DBIMPL_H
#define PANDASQL_DBIMPL_H

#include <map>
#include <string>

#include "Catalog/Column.h"
#include "Catalog/Table.h"
#include "Catalog/TableCatalog.h"

#include "Optimizer/Join/Join.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class BooleanExpr;
class IDBBackend;
class TupleIterator;

class DBImpl
{
public:

	DBImpl(StorageType inStorageType);
	~DBImpl();

	Status Open(const std::string &inDBPath, const OpenOptions &inOptions);
	Status Close();

	Status CreateOpenTable(const std::string &tableName, const std::string &creationStmt);
	Status OpenTable(const std::string &tableName, const ColumnDefList &columnList);
	Status DropTable(const std::string &tableName);

	Status InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList);
	Status DeleteData(const std::string &tableName, const BooleanExpr *inBooleanExpr = NULL);
	Status SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr = NULL);

	Status GetTableByName(const std::string &name, Table **o_table) const;

	Table* GetTableByID(uint32_t inTableID) const;
	uint32_t GetTableIDByName(const std::string &inTableName) const;
	uint32_t GetColumnIDByName(const std::string &inColumnName) const;

	Status GetColumnDefFromQualifiedName(const Table::TableRefList &inTableRefList, const ColumnQualifiedName &inQualifiedName, ColumnDef *io_columnDef) const;

	TupleIterator* CreateTupleIteratorForTable(const Table &inTable, const TupleDesc &inTupleDesc);

private:

	DBImpl(const DBImpl &rhs);
	DBImpl& operator=(const DBImpl &rhs);

	typedef std::map<std::string, Table*> TableMap;
	typedef std::pair<std::string, Table*> TableMapEntry;

	Status	OpenTableWithCreationStmt_Private(const std::string &inCreationStmt);
	void	ClearTableMap_Private();

	StorageType mStorageType;
	IDBBackend *mpBackend;

	TableCatalog mTableCatalog;

	TableMap mTableMap;

	bool mIsOpen;
	
};

}	// PandaSQL

#endif	// PANDASQL_DBIMPL_H