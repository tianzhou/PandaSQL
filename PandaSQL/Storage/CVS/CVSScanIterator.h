#ifndef PANDASQL_CVS_SCANITERATOR_H
#define PANDASQL_CVS_SCANITERATOR_H

#include "Storage/StorageTypes.h"

#include "Access/Iterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class PageProxy;

class CVSScanIterator : public Iterator
{
public:
	CVSScanIterator(const TuplePredicate *inPredicate, PageProxy *io_pageProxy);
	virtual ~CVSScanIterator();

	virtual bool Valid() const;
	virtual Status SeekToFirst();
	virtual Status SeekAfterLast();
	//virtual Status SeekToPredicate(const TuplePredicate *inPredicate);
	//virtual Status SeekToKey(const std::string &inKey);
	virtual Status Next();
	virtual Status Prev();
	//virtual Status GetKey(std::s tring *o_key) const;
	virtual Status InsertValue(const TupleData &inTuple);
	virtual Status UpdateValue(const TupleData &inTuple);
	virtual Status DeleteValue();
	virtual Status GetValue(TupleData *o_tuple) const;
	virtual Status GetValue(std::string *o_rowString) const;

protected:
	 
	struct PosInfo
	{
		PageNum pageNum;
		uint32_t offset; //Offset in page
	};
 
	CVSScanIterator(const CVSScanIterator &rhs);
	CVSScanIterator& operator=(const CVSScanIterator &rhs);

	Status Next_Inner();

	PageProxy *mpPageProxy;

	PosInfo mSeekPos;
	PosInfo mValuePos;
	bool mPosAfterLast;
};

}	// PandaSQL

#endif	// PANDASQL_CVS_SCANITERATOR_H