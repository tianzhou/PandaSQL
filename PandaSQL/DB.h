#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Catalog/Table.h"
#include "Catalog/TableCatalog.h"

#include "Utils/Status.h"

#include <vector>

namespace PandaSQL
{

class IVFS;
class File;
class Predicate;

class DB
{
public:

	typedef std::vector<Table*> TableList;
	
	struct Options
	{
		bool create_if_missing;

		Options();
	};

	DB();
	~DB();
	Status Open(const std::string &inDBPath, const Options &inOptions);
	Status Close();
	Status CreateTable(const std::string &inCreateStmt);

	//Load pTable to DB, Take the ownership of pTable;
	Status LoadTable(Table *pTable);

	Status InsertData(const std::string &tableName, const Table::ColumnRefList &columnList, const Table::ColumnValueList &columnValueList);
	Status DeleteData(const std::string &tableName, const Predicate *inPredicate = NULL);
	Status SelectData(const Table::TableRefList &tableList, const Table::ColumnRefList &columnList, const Predicate *inPredicate = NULL);

	const std::string& GetDBPath() const { return mDBPath; }
	IVFS* GetVFS() { return mpVFS; }

	Table* GetTableByID(uint32_t inTableID) const;
	uint32_t GetTableIDByName(const std::string &inTableName) const;
	uint32_t GetColumnIDByName(const std::string &inColumnName) const;

	Status GetTableByName(const std::string &name, Table **o_table) const;

private:
	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	std::string mDBPath;
	IVFS	*mpVFS;
	File *mpMainFile;
	File *mpTableFile;
	std::vector<File*> mDataFileList;
	std::vector<File*> mIndexFileList;

	TableCatalog mTableCatalog;

	TableList mTableList;
};

}	// PandaSQL

#endif	// PANDASQL_DB_H