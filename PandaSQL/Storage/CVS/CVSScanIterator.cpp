#include "stdafx.h"

#include "CVSScanIterator.h"

namespace PandaSQL
{

CVSScanIterator::CVSScanIterator(File *io_file)
:mpFile(io_file)
,mPosValid(false)
{
	mFilePos.mark = kBegin;
	mFilePos.key = std::string();
	mFilePos.offset = 0;
}

CVSScanIterator::~CVSScanIterator()
{
}

bool CVSScanIterator::Valid() const
{
	return false;
}

Status CVSScanIterator::SeekToFirst()
{
	mFilePos.mark = kBegin;
	mFilePos.key = std::string();
	mFilePos.offset = 0;
	mPosValid = false;

	return mStatus;
}

Status CVSScanIterator::SeekToLast()
{
	mFilePos.mark = kEnd;
	mFilePos.key = std::string();
	mFilePos.offset = 0;
	mPosValid = false;

	return mStatus;
}

Status CVSScanIterator::SeekToKey(const std::string &inKey)
{
	mFilePos.mark = kNormal;
	mFilePos.key = inKey;
	mFilePos.offset = 0;
	mPosValid = false;

	return mStatus;
}

Status CVSScanIterator::Next()
{
	return mStatus;
}

Status CVSScanIterator::Prev()
{
	return mStatus;
}

Status CVSScanIterator::GetKey(std::string *o_key) const
{
	return mStatus;
}

Status CVSScanIterator::GetValue(std::string *o_value) const
{
	return mStatus;
}

}	// PandaSQL