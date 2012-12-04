#ifndef PANDASQL_DBIMPL_H
#define PANDASQL_DBIMPL_H

#include <map>
#include <string>

#include "Catalog/Column.h"
#include "Catalog/Index.h"
#include "Catalog/IndexCatalog.h"
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

	Status CreateOpenTable(const std::string &tableName, const ColumnDefList &columnList, const std::string &creationStmt);
	Status OpenTable(const std::string &tableName, const ColumnDefList &columnList);
	Status DropTable(const std::string &tableName);
	
	Status CreateOpenIndex(const std::string &indexName, const std::string &tableName, const ColumnDefList &columnList, bool isUnique, const std::string &creationStmt);
	Status OpenIndex(const std::string &indexName, const std::string &tableName, const ColumnDefList &columnList, bool isUnique);
	Status DropIndex(const std::string &indexName, const std::string &tableName);

	Status InsertData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList);
	Status UpdateData(const std::string &tableName, const ColumnDefList &columnList, const ExprList &columnExprList, const BooleanExpr *inPredicateExpr = NULL);
	Status DeleteData(const std::string &tableName, const BooleanExpr *inPredicateExpr = NULL);
	Status SelectData(const Table::TableRefList &tableList, const JoinList &joinList, const ColumnDefList &projectColumnList, const BooleanExpr *inWhereExpr = NULL);

	Status GetTableByName(const std::string &tableName, const Table **o_table) const;

	Table* GetTableByID(uint32_t inTableID) const;
	uint32_t GetTableIDByName(const std::string &inTableName) const;
	uint32_t GetColumnIDByName(const std::string &inColumnName) const;

	Status GetColumnDefFromQualifiedName(const Table::TableRefList &inTableRefList, const ColumnQualifiedName &inQualifiedName, ColumnDef *io_columnDef) const;

	TupleIterator* CreateTupleIteratorForTable(const Table &inTable, const TupleDesc &inTupleDesc);

private:

	DBImpl(const DBImpl &rhs);
	DBImpl& operator=(const DBImpl &rhs);

	Status  Close_Private(bool forceClose);

	Status	OpenTableWithCreationStmt_Private(const std::string &inCreationStmt);
	void	AddTable_Private(const std::string &tableName, const ColumnDefList &columnList);

	Status	OpenIndexWithCreationStmt_Private(const std::string &inCreationStmt);
	void	AddIndex_Private(const std::string &indexName, const std::string &tableName, std::vector<int32_t> indexList, bool isUnique);

	typedef Status (*PerformIterator)(TupleIterator *io_iterator, void *io_ctx);
	Status	PerformIterator_Private(const std::string &tableName, const BooleanExpr *inPredicateExpr, PerformIterator performer, void *io_ctx);

	StorageType mStorageType;
	IDBBackend *mpBackend;

	TableCatalog mTableCatalog;
	IndexCatalog mIndexCatalog;

	bool mIsOpen;
	
};

}	// PandaSQL

#endif	// PANDASQL_DBIMPL_H