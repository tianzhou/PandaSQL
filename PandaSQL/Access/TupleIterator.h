#ifndef PANDASQL_TUPLE_ITERATOR_H
#define PANDASQL_TUPLE_ITERATOR_H

#include "Tuple.h"

#include "Catalog/Column.h"

#include "Utils/Status.h"
#include "Utils/Value.h"

namespace PandaSQL
{

class TupleIterator
{

public:

	TupleIterator(const ColumnDefList &inColumnDefList);
	virtual ~TupleIterator() = 0 {}

	virtual bool Valid() const = 0;
	virtual Status SeekToFirst() = 0;
	virtual Status Next() = 0;
	virtual Status Prev() = 0;
	virtual Status GetValue(ValueList *o_valueList) const = 0;

	Status GetLastError() const { return mLastError; }

protected:

	mutable Status mLastError;
	const ColumnDefList &mColumnDefList;
};

}	// PandaSQL


#endif //PANDASQL_TUPLE_ITERATOR_H
