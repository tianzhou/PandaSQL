#ifndef PANDASQL_CVS_SCANITERATOR_H
#define PANDASQL_CVS_SCANITERATOR_H

#include "Storage/Iterator.h"

#include "Utils/Types.h"

namespace PandaSQL
{

class File;

class CVSScanIterator : public Iterator
{
public:
	CVSScanIterator(File *io_file);
	virtual ~CVSScanIterator();

	virtual bool Valid() const;
	virtual Status SeekBeforeFirst();
	virtual Status SeekAfterLast();
	//virtual Status SeekToKey(const std::string &inKey);
	virtual Status Next();
	virtual Status Prev();
	//virtual Status GetKey(std::string *o_key) const;
	virtual Status GetValue(std::string *o_value) const;

protected:

	enum IterPosMark
	{
		kBegin,
		kEnd,
		kNormal
	};
	 
	struct PosInfo
	{
		IterPosMark mark;
		uint32_t offset; //Offset relative to mark&key
	};

	CVSScanIterator(const CVSScanIterator &rhs);
	CVSScanIterator& operator=(const CVSScanIterator &rhs);

	File *mpFile;

	PosInfo mFilePos;
	bool mPosValid;

	std::string mCurentData;
};

}	// PandaSQL

#endif	// PANDASQL_CVS_SCANITERATOR_H