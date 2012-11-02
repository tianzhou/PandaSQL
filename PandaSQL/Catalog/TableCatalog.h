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

	const Table* GetTableByName(const std::string &inTableName) const;
	
	//Don't return value, instead it will throw assert
	//It's an internal class, so the caller should make sure
	//not adding duplicate table or not removing non-existing table
	void AddTable(const std::string &inTableName, Table *io_table);
	void RemoveTable(const std::string &inTableName);

private:

	typedef std::map<std::string, Table*> TableStore;

	TableStore mTableStore;
	
	
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_CATALOG_H