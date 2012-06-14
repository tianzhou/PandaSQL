#include "stdafx.h"

#include "TableCatalog.h"
#include "Table.h"

namespace PandaSQL
{

TableCatalog::TableCatalog()
{
}

TableCatalog::~TableCatalog()
{
}

Table* TableCatalog::GetTableByName(const std::string &inTableName)
{
	return mTableStore[inTableName];
}

void   TableCatalog::AddTableWithName(const std::string &inTableName, Table *io_table)
{
	//Prevent adding duplicate
	PDASSERT(mTableStore.find(inTableName) == mTableStore.end());
	mTableStore[inTableName] = io_table;
}

void   TableCatalog::RemoveTableWithName(const std::string &inTableName)
{
	TableStore::const_iterator iter = mTableStore.find(inTableName);

	//Must exist
	PDASSERT(iter != mTableStore.end());
	mTableStore.erase(iter);
}

}	// PandaSQL
