#ifndef PANDASQL_ISTORAGE_H
#define PANDASQL_ISTORAGE_H

#include "Utils/Status.h"
#include "Utils/Types.h"

namespace PandaSQL
{

class ITuple;

class IStorage
{
public:

	enum StorageType
	{
		kCVS
	};

	static IStorage *CreateStorage(StorageType inType);

	virtual ~IStorage() = 0 {}

	virtual Status OpenTable(const std::string &inTableName) = 0;
	virtual Status InsertRow(const ITuple &inTuple) = 0;

protected:
	IStorage();

private:
	IStorage(const IStorage &rhs);
	IStorage& operator=(const IStorage &rhs);
};

class IStorageHelper
{
public:

	virtual ~IStorageHelper() = 0 {}

protected:
	IStorageHelper();

private:
	IStorageHelper(const IStorageHelper &rhs);
	IStorageHelper& operator=(const IStorageHelper &rhs);

};

}	// PandaSQL

#endif	// PANDASQL_ISTORAGE_H