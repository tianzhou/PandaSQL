#ifndef PANDASQL_INDEX_H
#define PANDASQL_INDEX_H

#include <string>
#include <vector>

#include "Utils/Types.h"

namespace PandaSQL
{

class Index
{

public:

	Index(void *payload);
	~Index();

	void* GetPayload() const { return mPayload; }
	
	void SetIndexName(const std::string &inName) { mIndexName = inName; }
	std::string GetIndexName() const { return mIndexName; }

	void SetTableName(const std::string &inName) { mTableName = inName; }
	std::string GetTableName() const { return mTableName; }

	void SetColumnIndexList(const UInt32List &inColumnIndexList) { mColumnIndexList = inColumnIndexList; }
	const UInt32List& GetColumnIndexList() const { return mColumnIndexList; }

	void SetIsUnique(bool inUnique) { mUnique = inUnique; }
	bool GetIsUnique() const { return mUnique; }

private:
	
	Index(const Index &rhs);
	Index& operator=(const Index &rhs);

	//payload is not owned by this class, 
	//so it's reasonable to make it mutable
	mutable void *mPayload;

	std::string mIndexName;
	std::string mTableName;
	UInt32List mColumnIndexList;
	bool mUnique;
};

}	// PandaSQL

#endif	// PANDASQL_INDEX_H