#ifndef PANDASQL_BDB_SCANITERATOR_H
#define PANDASQL_BDB_SCANITERATOR_H

#include <db_cxx.h>

#include "Access/TupleIterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class BDBScanIterator : public TupleIterator
{
public:

	BDBScanIterator(const TupleDesc &inTupleDesc, DB *io_dbTable, DB_ENV *io_dbEnv);
	virtual ~BDBScanIterator();

	virtual bool Valid() const;
	virtual void Reset();
	virtual bool Next();
	virtual bool Prev();
	virtual bool GetValue(ValueList *o_tupleValueList) const;
	virtual bool Remove();

protected:
 
	BDBScanIterator(const BDBScanIterator &rhs);
	BDBScanIterator& operator=(const BDBScanIterator &rhs);

	bool MoveCursor_Private(u_int32_t flags);

private:
	
	DB *mpDBTable;
	DBC *mpDBCursor;
	DB_TXN *mpDBTXN;

	bool mJustReset;
};

}	// PandaSQL

#endif	// PANDASQL_BDB_SCANITERATOR_H