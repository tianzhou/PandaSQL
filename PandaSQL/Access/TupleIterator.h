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
	void   SetTupleFunctor(TupleFunctor *io_tupleFunctor) { mpTupleFunctor = io_tupleFunctor; }

	virtual bool Valid() const = 0;
	virtual void Reset() = 0;
	virtual bool Next() = 0;
	virtual bool Prev() = 0;
	virtual bool GetValue(ValueList *o_valueList) const = 0;

protected:

	TupleFunctor *mpTupleFunctor;
	const TupleDesc &mTupleDesc;

	mutable Status mLastError;
};

}	// PandaSQL


#endif //PANDASQL_TUPLE_ITERATOR_H
