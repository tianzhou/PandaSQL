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

	void SetIndexList(const std::vector<int32_t> &inIndexList) { mIndexList = inIndexList; }
	const std::vector<int32_t>& GetIndexList() const { return mIndexList; }

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
	std::vector<int32_t> mIndexList;
	bool mUnique;
};

}	// PandaSQL

#endif	// PANDASQL_INDEX_H