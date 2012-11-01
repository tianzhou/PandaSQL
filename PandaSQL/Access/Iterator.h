#ifndef PANDASQL_ITERATOR_H
#define PANDASQL_ITERATOR_H

#include <string>

#include "Access/Tuple.h"

#include "Utils/Status.h"

namespace PandaSQL
{

class TuplePredicate;

class Iterator
{
public:

	virtual ~Iterator() = 0 {}

	virtual bool Valid() const = 0;
	virtual Status SeekToFirst() = 0;
	virtual Status SeekAfterLast() = 0;
	//virtual Status SeekToPredicate(const TuplePredicate *inTuplePredicate) = 0;
	//virtual Status SeekToKey(const std::string &inKey) = 0;
	virtual Status Next() = 0;
	virtual Status Prev() = 0;
	//virtual Status GetKey(std::string *o_key) const = 0;
	virtual Status InsertValue(const ValueList &inTupleValueList) = 0;
	virtual Status UpdateValue(const ValueList &inTupleValueList) = 0;
	virtual Status DeleteValue() = 0;
	virtual Status GetValue(ValueList *o_tupleValueList) const = 0;

	Status GetStatus() const { return mStatus; }

protected:

	Iterator(const TupleDesc &inTupleDesc, const TuplePredicate *inTuplePredicate);

	Iterator(const Iterator &rhs);
	Iterator& operator=(const Iterator &rhs);

	Status mStatus;
	const TupleDesc	&mpTupleDesc;
	const TuplePredicate *mpTuplePredicate;
};

}	// PandaSQL

#endif	// PANDASQL_ITERATOR_H