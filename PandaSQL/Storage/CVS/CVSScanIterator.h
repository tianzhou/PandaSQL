#ifndef PANDASQL_CVS_SCANITERATOR_H
#define PANDASQL_CVS_SCANITERATOR_H

#include "Storage/Iterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class PageProxy;

class CVSScanIterator : public Iterator
{
public:
	CVSScanIterator(PageProxy *io_pageProxy);
	virtual ~CVSScanIterator();

	virtual bool Valid() const;
	virtual Status SeekToFirst();
	virtual Status SeekAfterLast();
	//virtual Status SeekToKey(const std::string &inKey);
	virtual Status Next();
	virtual Status Prev();
	//virtual Status GetKey(std::s tring *o_key) const;
	virtual Status InsertValue(const ITuple &inTuple);
	virtual Status UpdateValue(const ITuple &inTuple);
	virtual Status GetValue(ITuple *o_tuple) const;

protected:
	 
	struct PosInfo
	{
		uint32_t pageNum;
		uint32_t offset; //Offset in page
	};
 
	CVSScanIterator(const CVSScanIterator &rhs);
	CVSScanIterator& operator=(const CVSScanIterator &rhs);

	Status Next_Inner();

	PageProxy *mpPageProxy;

	PosInfo mSeekPos;
	PosInfo mValuePos;
	bool mPosAfterLast;

	std::string mCurentData;
};

}	// PandaSQL

#endif	// PANDASQL_CVS_SCANITERATOR_H