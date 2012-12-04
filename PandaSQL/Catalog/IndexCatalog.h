#ifndef PANDASQL_INDEX_CATALOG_H
#define PANDASQL_INDEX_CATALOG_H

#include <map>

namespace PandaSQL
{

class Index;

class IndexCatalog
{

public:

	IndexCatalog();
	~IndexCatalog();

	const Index* GetIndexByName(const std::string &inIndexName, const std::string &inTableName) const;
	
	//Don't return value, instead it will throw assert
	//It's an internal class, so the caller should make sure
	//not adding duplicate index or not removing non-existing index
	void AddIndex(const std::string &inIndexName, const std::string &inTableName, Index *io_index);
	void RemoveIndex(const std::string &inIndexName, const std::string &inTableName);

private:

	struct IndexEntryKey
	{
		std::string indexName;
		std::string tableName;
		bool operator==(const IndexEntryKey &rhs) const;
		bool operator<(const IndexEntryKey &rhs) const;
	};

	typedef std::map<IndexEntryKey, Index*> IndexStore;

	IndexStore mIndexStore;
	
	
};

}	// PandaSQL

#endif	// PANDASQL_TABLE_CATALOG_H