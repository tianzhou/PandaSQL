#ifndef PANDASQL_TABLE_CATALOG_H
#define PANDASQL_TABLE_CATALOG_H

#include <map>

namespace PandaSQL
{

class Table;

class TableCatalog
{

public:

	TableCatalog();
	~TableCatalog();

	Table* GetTableByName(const std::string &inTableName);
	void   AddTableWithName(const std::string &inTableName, Table *io_table);
	void   RemoveTableWithName(const std::string &inTableName);

private:

	typedef std::map<std::string, Table*> TableStore;

	TableStore mTableStore;
	
	
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_CATALOG_H