#ifndef PANDASQL_BDB_INDEX_SCANITERATOR_H
#define PANDASQL_BDB_INDEX_SCANITERATOR_H

#include <db_cxx.h>

#include "Access/TupleIterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class BDBIndexScanIterator : public TupleIterator
{
public:

	BDBIndexScanIterator(const UInt32List &inTupleIndexList, const TupleDesc &inTupleDesc, DB *io_dbTable, DB_ENV *io_dbEnv);
	virtual ~BDBIndexScanIterator();

	virtual bool Valid() const;
	virtual void Reset();
	virtual bool Next();
	virtual bool Prev();
	virtual bool First();
	virtual bool Last();
	virtual bool GetValue(ValueList *o_tupleValueList) const;
	virtual bool Update(const ValueList &inValueList);
	virtual bool Remove();

protected:
 
	BDBIndexScanIterator(const BDBIndexScanIterator &rhs);
	BDBIndexScanIterator& operator=(const BDBIndexScanIterator &rhs);

	bool GetCursor_Private(u_int32_t flags);
	bool PutCursor_Private(const ValueList &inValueList, u_int32_t flags);

private:
	
	DB *mpDBTable;
	DBC *mpDBCursor;
	DB_TXN *mpDBTXN;

	bool mJustReset;
	bool mInvalidCursor;

	const UInt32List mTupleIndexList;
};

}	// PandaSQL

#endif	// PANDASQL_BDB_INDEX_SCANITERATOR_H