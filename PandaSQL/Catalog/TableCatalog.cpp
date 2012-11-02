#include "stdafx.h"

#include "Catalog/TableCatalog.h"

#include "Catalog/Table.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

TableCatalog::TableCatalog()
{
}

TableCatalog::~TableCatalog()
{
}

const Table* TableCatalog::GetTableByName(const std::string &inTableName) const
{
	const Table* returnedTable = NULL;
	TableStore::const_iterator iter = mTableStore.find(inTableName);
	
	if (iter != mTableStore.end())
	{
		returnedTable = iter->second;
	}

	return returnedTable;
}

void   TableCatalog::AddTable(const std::string &inTableName, Table *io_table)
{
	//Prevent adding duplicate
	PDASSERT(mTableStore.find(inTableName) == mTableStore.end());
	mTableStore[inTableName] = io_table;
}

void   TableCatalog::RemoveTable(const std::string &inTableName)
{
	TableStore::const_iterator iter = mTableStore.find(inTableName);

	//Must exist
	PDASSERT(iter != mTableStore.end());
	mTableStore.erase(iter);
}

}	// PandaSQL
