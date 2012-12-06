#ifndef PANDASQL_TUPLE_ITERATOR_H
#define PANDASQL_TUPLE_ITERATOR_H

#include "Access/Tuple.h"

#include "Utils/Status.h"
#include "Utils/Value.h"

namespace PandaSQL
{

class TupleIterator
{

public:

	TupleIterator(const TupleDesc &inTupleDesc);
	virtual ~TupleIterator() = 0 {}

	Status GetLastError() const { return mLastError; }

	virtual bool Valid() const = 0;
	virtual void Reset() = 0;
	virtual bool Next() = 0;
	virtual bool Prev() = 0;
	virtual bool Last() = 0;
	virtual bool GetValue(ValueList *o_tupleValueList) const = 0;
	virtual bool Update(const ValueList &inValueList) = 0;
	virtual bool Remove() = 0;

protected:

	const TupleDesc &mTupleDesc;

	mutable Status mLastError;
};

}	// PandaSQL


#endif //PANDASQL_TUPLE_ITERATOR_H
