#include "stdafx.h"

#include "Catalog/IndexCatalog.h"

#include "Catalog/Index.h"

#include "Utils/Debug.h"

namespace PandaSQL
{

bool IndexCatalog::IndexEntryKey::operator==(const IndexEntryKey &rhs) const
{
	return this->indexName == rhs.indexName
		&& this->tableName == rhs.tableName
		;
}

bool IndexCatalog::IndexEntryKey::operator<(const IndexEntryKey &rhs) const
{
	if (this->tableName == rhs.tableName)
	{
		return this->indexName < rhs.indexName;
	}

	return this->tableName < rhs.tableName;
}

IndexCatalog::IndexCatalog()
{
}

IndexCatalog::~IndexCatalog()
{
}

const Index* IndexCatalog::GetIndexByName(const std::string &inIndexName, const std::string &inTableName) const
{
	const Index* returnedIndex = NULL;

	IndexEntryKey entryKey;
	entryKey.indexName = inIndexName;
	entryKey.tableName = inTableName;

	IndexStore::const_iterator iter = mIndexStore.find(entryKey);
	
	if (iter != mIndexStore.end())
	{
		returnedIndex = iter->second;
	}

	return returnedIndex;
}

void   IndexCatalog::AddIndex(const std::string &inIndexName, const std::string &inTableName, Index *io_index)
{
	IndexEntryKey entryKey;
	entryKey.indexName = inIndexName;
	entryKey.tableName = inTableName;

	//Prevent adding duplicate
	PDASSERT(mIndexStore.find(entryKey) == mIndexStore.end());
	mIndexStore[entryKey] = io_index;
}

void   IndexCatalog::RemoveIndex(const std::string &inIndexName, const std::string &inTableName)
{
	IndexEntryKey entryKey;
	entryKey.indexName = inIndexName;
	entryKey.tableName = inTableName;

	IndexStore::const_iterator iter = mIndexStore.find(entryKey);

	//Must exist
	PDASSERT(iter != mIndexStore.end());
	mIndexStore.erase(iter);
}

}	// PandaSQL
