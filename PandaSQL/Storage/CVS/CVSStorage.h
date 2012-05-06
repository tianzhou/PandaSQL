#ifndef PANDASQL_CVSSTORAGE_H
#define PANDASQL_CVSSTORAGE_H

#include "Storage/IStorage.h"

namespace PandaSQL
{

class ITuple;

class CVSStorage : public IStorage
{
public:

	CVSStorage();
	virtual ~CVSStorage();

	virtual Status OpenTable(const std::string &inTableName);
	virtual Status InsertRow(const ITuple &inTuple);

private:
	CVSStorage(const CVSStorage &rhs);
	CVSStorage& operator=(const CVSStorage &rhs);
};

}	// PandaSQL

#endif	// PANDASQL_CVSSTORAGE_H