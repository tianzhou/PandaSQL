#ifndef PANDASQL_BDB_SEQ_SCANITERATOR_H
#define PANDASQL_BDB_SEQ_SCANITERATOR_H

#include <db_cxx.h>

#include "Access/TupleIterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class BDBSeqScanIterator : public TupleIterator
{
public:

	BDBSeqScanIterator(const TupleDesc &inTupleDesc, DB *io_dbTable, DB_ENV *io_dbEnv);
	virtual ~BDBSeqScanIterator();

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
 
	BDBSeqScanIterator(const BDBSeqScanIterator &rhs);
	BDBSeqScanIterator& operator=(const BDBSeqScanIterator &rhs);

	bool GetCursor_Private(u_int32_t flags);
	bool PutCursor_Private(const ValueList &inValueList, u_int32_t flags);

private:
	
	DB *mpDBTable;
	DBC *mpDBCursor;
	DB_TXN *mpDBTXN;

	bool mJustReset;
	bool mInvalidCursor;
};

}	// PandaSQL

#endif	// PANDASQL_BDB_SEQ_SCANITERATOR_H