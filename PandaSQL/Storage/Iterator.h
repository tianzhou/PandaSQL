#ifndef PANDASQL_ITERATOR_H
#define PANDASQL_ITERATOR_H

#include "Utils/Status.h"

namespace PandaSQL
{

class Iterator
{
public:

	virtual ~Iterator() = 0 {}

	virtual bool Valid() const = 0;
	virtual Status SeekBeforeFirst() = 0;
	virtual Status SeekAfterLast() = 0;
	//virtual Status SeekToKey(const std::string &inKey) = 0;
	virtual Status Next() = 0;
	virtual Status Prev() = 0;
	//virtual Status GetKey(std::string *o_key) const = 0;
	virtual Status GetValue(std::string *o_value) const = 0;

	Status GetStatus() const { return mStatus; }

protected:

	Iterator();

	Iterator(const Iterator &rhs);
	Iterator& operator=(const Iterator &rhs);

	Status mStatus;
};

}	// PandaSQL

#endif	// PANDASQL_ITERATOR_H