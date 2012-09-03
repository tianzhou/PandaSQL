#ifndef PANDASQL_BDB_SCANITERATOR_H
#define PANDASQL_BDB_SCANITERATOR_H

#include "Access/TupleIterator.h"

#include "Utils/Types.h"

#include <db_cxx.h>

namespace PandaSQL
{

class BDBScanIterator : public TupleIterator
{
public:

	BDBScanIterator(const ColumnDefList &inColumnDefList, DB *io_dbTable);
	virtual ~BDBScanIterator();

	virtual bool Valid() const;
	virtual Status SeekToFirst();
	virtual Status Next();
	virtual Status Prev();
	virtual Status GetValue(ValueList *o_valueList) const;

protected:
 
	BDBScanIterator(const BDBScanIterator &rhs);
	BDBScanIterator& operator=(const BDBScanIterator &rhs);

	Status MoveCursor_Private(u_int32_t flags);

private:
	
	DB *mpDBTable;
	DBC *mpDBCursor;
};

}	// PandaSQL

#endif	// PANDASQL_BDB_SCANITERATOR_H