#ifndef PANDASQL_DB_H
#define PANDASQL_DB_H

#include "Catalog/Table.h"

#include "Utils/Status.h"

#include <vector>

namespace PandaSQL
{

class VFS;
class File;

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

	const std::string& GetDBPath() const { return mDBPath; }
	VFS* GetVFS() { return mpVFS; }

private:
	DB(const DB &rhs);
	DB& operator=(const DB &rhs);

	Status GetTableByName(const std::string &name, Table **o_table) const;

	std::string mDBPath;
	VFS	*mpVFS;
	File *mpMainFile;
	File *mpTableFile;
	std::vector<File*> mDataFileList;
	std::vector<File*> mIndexFileList;

	TableList mTableList;
};

}	// PandaSQL

#endif	// PANDASQL_DB_H