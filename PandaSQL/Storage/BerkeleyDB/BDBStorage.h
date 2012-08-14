#ifndef PANDASQL_BDBSTORAGE_H
#define PANDASQL_BDBSTORAGE_H

#include "Storage/IStorage.h"

namespace PandaSQL
{

class BDBStorage : public IStorage
{
public:

	BDBStorage(const std::string &inRootPath);
	virtual ~BDBStorage();

	virtual Iterator* CreateScanIterator(const TuplePredicate *inTuplePredicate = NULL);
	virtual Iterator* CreateIndexIterator();

	virtual void ReleaseScanIterator(Iterator *iter);
	virtual void ReleaseIndexIterator(Iterator *iter); 

	virtual Status OpenTable(const std::string &inTableName, OpenMode inMode);
	//virtual Status InsertRecord(const TupleData &inTuple);
	//virtual Status FindFirstRecordWithPredicate(const Predicate *inPredicate, Iterator **o_iterator);

private:

	BDBStorage(const BDBStorage &rhs);
	BDBStorage& operator=(const BDBStorage &rhs);

	OpenMode mDataFileMode;

	Iterator *mpScanIterator;
};

}	// PandaSQL

#endif	// PANDASQL_BDBSTORAGE_H