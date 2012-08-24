#ifndef PANDASQL_BDB_SCANITERATOR_H
#define PANDASQL_BDB_SCANITERATOR_H

#include "Access/Iterator.h"

#include "Utils/Types.h"

#include <db_cxx.h>

namespace PandaSQL
{

class BDBScanIterator : public Iterator
{
public:

	BDBScanIterator(const TupleDesc &inTupleDesc, const TuplePredicate *inPredicate, DB *io_dbTable);
	virtual ~BDBScanIterator();

	virtual bool Valid() const;
	virtual Status SeekToFirst();
	virtual Status SeekAfterLast();
	//virtual Status SeekToPredicate(const TuplePredicate *inPredicate);
	//virtual Status SeekToKey(const std::string &inKey);
	virtual Status Next();
	virtual Status Prev();
	//virtual Status GetKey(std::s tring *o_key) const;
	virtual Status InsertValue(const TupleData &inTuple);
	virtual Status UpdateValue(const TupleData &inTuple);
	virtual Status DeleteValue();
	virtual Status GetValue(TupleData *o_tuple) const;

protected:
 
	BDBScanIterator(const BDBScanIterator &rhs);
	BDBScanIterator& operator=(const BDBScanIterator &rhs);

	Status MoveCursor_Private(u_int32_t flags);

private:
	
	DB *mpDBTable;
	DBC *mpDBCursor;
	mutable Status mLastError;
};

}	// PandaSQL

#endif	// PANDASQL_BDB_SCANITERATOR_H