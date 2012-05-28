#ifndef PANDASQL_ITERATOR_H
#define PANDASQL_ITERATOR_H

#include "Utils/Status.h"

namespace PandaSQL
{

class ITuple;

class Iterator
{
public:

	virtual ~Iterator() = 0 {}

	virtual bool Valid() const = 0;
	virtual Status SeekToFirst() = 0;
	virtual Status SeekAfterLast() = 0;
	//virtual Status SeekToKey(const std::string &inKey) = 0;
	virtual Status Next() = 0;
	virtual Status Prev() = 0;
	//virtual Status GetKey(std::string *o_key) const = 0;
	virtual Status InsertValue(const ITuple &inTuple) = 0;
	virtual Status UpdateValue(const ITuple &inTuple) = 0;
	virtual Status GetValue(ITuple *o_tuple) const = 0;

	Status GetStatus() const { return mStatus; }

protected:

	Iterator();

	Iterator(const Iterator &rhs);
	Iterator& operator=(const Iterator &rhs);

	Status mStatus;
};

}	// PandaSQL

#endif	// PANDASQL_ITERATOR_H