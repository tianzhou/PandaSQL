#ifndef PANDASQL_ITERATOR_H
#define PANDASQL_ITERATOR_H

#include "Utils/Status.h"

namespace PandaSQL
{

class Tuple;
class Predicate;

class Iterator
{
public:

	virtual ~Iterator() = 0 {}

	virtual bool Valid() const = 0;
	virtual Status SeekToFirst() = 0;
	virtual Status SeekAfterLast() = 0;
	virtual Status SeekToPredicate(const Predicate *inPredicate) = 0;
	//virtual Status SeekToKey(const std::string &inKey) = 0;
	virtual Status Next() = 0;
	virtual Status Prev() = 0;
	//virtual Status GetKey(std::string *o_key) const = 0;
	virtual Status InsertValue(const Tuple &inTuple) = 0;
	virtual Status UpdateValue(const Tuple &inTuple) = 0;
	virtual Status DeleteValue() = 0;
	virtual Status GetValue(Tuple *o_tuple) const = 0;

	Status GetStatus() const { return mStatus; }

protected:

	Iterator(const Predicate *inPredicate);

	Iterator(const Iterator &rhs);
	Iterator& operator=(const Iterator &rhs);

	Status mStatus;
	const Predicate *mpPredicate;
};

}	// PandaSQL

#endif	// PANDASQL_ITERATOR_H