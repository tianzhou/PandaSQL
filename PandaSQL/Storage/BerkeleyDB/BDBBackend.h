#ifndef PANDASQL_BDBBackend_H
#define PANDASQL_BDBBackend_H

#include "Storage/IDBBackend.h"

namespace PandaSQL
{

class BDBBackend : public IDBBackend
{
public:

	BDBBackend(const std::string &inRootPath);
	virtual ~BDBBackend();

	virtual Status CreateTable(const Table &inTable);
	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode);
	//virtual Status InsertRecord(const TupleData &inTuple) = 0;
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator) = 0;

protected:
	

private:
	BDBBackend(const BDBBackend &rhs);
	BDBBackend& operator=(const BDBBackend &rhs);
};


}	// PandaSQL

#endif	// PANDASQL_BDBBackend_H